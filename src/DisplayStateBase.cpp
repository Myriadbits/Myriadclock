///////////////////////////////////////////////////////////////////////////////
// DisplayStateBase class
// Base class for controlling the display/led states
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateBase.h"

// For now assume the (0,0) of the dutch coordinate system 
#define CLOCK_LATITUDE      52.155194
#define CLOCK_LONGITUDE     5.387270
#define CLOCK_ALTITUDE      10

#define YEAR_2000_TIMESTAMP     946684800ULL
#define SECONDS_PER_HOUR        3600ULL
#define SECONDS_PER_DAY         86400ULL

ledclocklayout_t DisplayStateBase::s_layout = s_layoutNL_V1;

//
//  Return the elapsed time in milliseconds since a timestamp
//
uint32_t DisplayStateBase::Elapsed(uint32_t ts)
{
    uint32_t now = millis();
    if (ts < now) return (now - ts);
    return(now + (0xffffffff - ts));
}

//
// Return the led index in the leds array
//
int16_t DisplayStateBase::CalcLedPos(int8_t x, int8_t y)
{
    if ((y % 2) == 0)
        return (NUM_COLS - x - 1) + (y * NUM_COLS); 
    else
        return x + (y * NUM_COLS); 
}

//
// Set the clock layout
void DisplayStateBase::setLayout(const ledclocklayout_t *layout) 
{      
    memcpy(&s_layout, layout, sizeof(s_layout));    
};

//
// Color handler for words
CRGB DisplayStateBase::ColorHandler(CRGB defaultColor, int brightness, int customParam)
{
    return defaultColor.fadeLightBy(255 - brightness);
    //return defaultColor.nscale8_video(brightness);
}

//
// Add a single word to the display/leds
// customParam can be any value, normal operation when ColorHandler is not overriden: customParam is the color
void DisplayStateBase::AddWordToLeds(const ledpos_t *pCurrentWord, CRGB defaultColor, int brightness, int customParam) 
{
    if (pCurrentWord == NULL) return;
    if (m_pLEDs == NULL) return;

    uint8_t charIndex = 0;
    ledpos_t ledPos = pCurrentWord[charIndex];
    while (ledPos.x >= 0 && ledPos.y >= 0)
    {
        // Led numbers are inverted left to right every other row:        
        m_pLEDs[CalcLedPos(ledPos.x, ledPos.y)] = ColorHandler(defaultColor, brightness, customParam);
        
        // Next char
        charIndex++;
        ledPos = pCurrentWord[charIndex];
    }
}

//
// Fill the entire background with a color
void DisplayStateBase::FillBackground(const int brightness)
{
    // Set the background color
    if (brightness != 0)
    {
        CRGB colDefault = m_pConfig->getConfigValue(CONFIG_COLOR_BACKGROUND);

        CRGB rgbClear = ColorHandler(colDefault, brightness, 0);
        for(int n = 0; n < NUM_LEDS; n++)
            m_pLEDs[n] = rgbClear;
    }
}

//
// Simple log function
void DisplayStateBase::log(const char* format, ...)
{
    static char sBuffer[256];
    va_list ap;
    va_start(ap, format);
    Serial.printf("[%s %lu] ", m_name.c_str(), millis());
    vsprintf(sBuffer, format, ap);
    Serial.println(sBuffer);
    va_end(ap);
}


//
// Calculate the sunrise or the sunset times for a specific date
// This is a first calculation with a aberration of max 3 minutes - for now it is OK. Otherwise, calcalution
// must be re-done with the last new time and put this value again in centuries_t value. 
// See documentation:  http://www.stargazing.net/kepler/sunrise.html
//
void DisplayStateBase::CalcSunriseSunset(unsigned long timestamp, float lat, float lon, float alt, unsigned long *sunrise, unsigned long *sunset)
{
    unsigned long today = (unsigned long) (timestamp / SECONDS_PER_DAY) * SECONDS_PER_DAY;
    double t = (unsigned long)((timestamp - YEAR_2000_TIMESTAMP ) / SECONDS_PER_DAY) - 0.5;
    double centuries = t / 36525;

    double l = (280.460 + (36000.770 * centuries)) + 360; // Mean longitude including aberration
    double g = (357.528 + (35999.050 * centuries)) + 360; // Mean anomaly

    double ec = (1.915 * sin(DEG_TO_RAD * g)) + (0.02 * sin(DEG_TO_RAD * g));  // ec centre correction
    double lambda = l + ec;                                                        // ecliptic longitude of Sun
    double e = -ec + (2.466 * sin(DEG_TO_RAD * 2 * lambda)) - (0.053 * sin(DEG_TO_RAD * 4 * lambda));

    //
    //  gha = UTo - 180 + e
    //  where UTo is the current estimate of the time of sunrise expressed in degrees. For this first iteration, this gives:
    //
    double gha = 180-180 + e;
    
    //
    //  We also need the Sun's declination(delta), for which we need the obliquity of the ecliptic (tilt of the Earth's axis)
    //
    double obl = 23.4393 - (0.0130 * centuries);
    double delta = asin((sin(DEG_TO_RAD * obl) * sin(DEG_TO_RAD * lambda))) * 180 / PI;     /* Sun's declination */
    
    //
    //  Correction term and new estimate of time
    //
    double cosc = (sin(DEG_TO_RAD * alt) - (sin(DEG_TO_RAD * lat) * sin(DEG_TO_RAD * delta))) / (cos(DEG_TO_RAD * lat) * cos(DEG_TO_RAD * delta));

    double correction = 0;
    if      (cosc >  1.0) { correction = 0;     }
    else if (cosc < -1.0) { correction = 180;   }
    else                  { correction = acos(cosc) * 180 / PI; }

    if (sunrise) *sunrise = today + (180.0 - (gha + lon + correction)) * 12 * SECONDS_PER_HOUR / 180.0 ;
    if (sunset ) *sunset  = today + (180.0 - (gha + lon - correction)) * 12 * SECONDS_PER_HOUR / 180.0 ;
}

//
// Auto adjust brightness depending upon sunrise/sunset
//
int DisplayStateBase::GetBrightness(unsigned long epochTime)
{
    unsigned long sunrise, sunset;
    CalcSunriseSunset(epochTime, CLOCK_LATITUDE, CLOCK_LONGITUDE, CLOCK_ALTITUDE, &sunrise, &sunset);

    //           sunrise            sunset        
    //  |        |                  |        |
    //  |    __--|       100%       |--__    |
    //  |__--                            --__|   30%

    int brightnessDay = 10; // Use some defaults
    int brightnessNight = 10;
    if (m_pConfig != NULL) 
    {
        brightnessDay = m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_DAY);
        brightnessNight = m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_NIGHT);
    }

    int brightness = brightnessNight;
    unsigned long deltaTime = 30 * 60;
    int brightnessDiff = (brightnessDay - brightnessNight);
    if (epochTime > sunrise - deltaTime && epochTime <= sunrise)
    {
        // Morning Twilight
        brightness =  brightnessNight + ((sunrise - epochTime) * brightnessDiff) / deltaTime;
    }
    else if (epochTime >= sunrise && epochTime <= sunset)
    {
        // Daytime
        brightness = brightnessDay;
    }
    else if (epochTime >= sunset && epochTime < sunset + deltaTime)
    {
        // Evening Twilight
        brightness = brightnessNight - ((epochTime - sunset) * brightnessDiff) / deltaTime;
    }    
    return brightness;
}
