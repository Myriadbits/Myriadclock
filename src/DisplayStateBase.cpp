///////////////////////////////////////////////////////////////////////////////
// DisplayStateBase class
// Base class for controlling the display/led states
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateBase.h"
#include "ClockLayoutNL_V1.h"
#include "Myriadbitsfont5x6_3.h"
#include "Myriadbitsfont5x8_3.h"
#include "HighFont4x8.h"
#include "HighFont4x6.h"

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
    int pos = 0;
    if (s_layout.orientation == EOrientation::O_NORMAL)
    {
        if ((y % 2) == 0)
            pos = (s_layout.columns - x - 1) + (y * s_layout.columns); 
        else
            pos = x + (y * s_layout.columns); 
    }
    else
    {
        if ((x % 2) == 0)
            pos = y + (x * s_layout.rows); 
        else
            pos = (s_layout.rows - y - 1) + (x * s_layout.rows); 
    }
    return (pos % s_layout.numLeds);
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
        for(int n = 0; n < s_layout.numLeds; n++)
            m_pLEDs[n] = rgbClear;
    }
}


//
// @brief  Draw a character on the LED panel
//
void DisplayStateBase::DrawGFX(CRGB *pLEDs, EFontType fontType, ETextAlign align, int x, int y, std::string text, CRGB color)
{
    const GFXfont* pFont = NULL;
    switch (fontType)
    {
        case EFontType::FT_56: pFont = &JochemFont56; break;
        case EFontType::FT_HIGHFONT48: pFont = &Highfont4x8; break;
        case EFontType::FT_HIGHFONT46: pFont = &Highfont4x6; break;
        case EFontType::FT_582: pFont = &JochemFont58; break;
    }
    if (pFont == NULL)
        return;

    int posX = x;
    int posY = y;

    if ((align & ETextAlign::TA_HCENTER) != 0)
    {
        int sizeX = CalculateGFXWidth(pFont, text);
        posX = (s_layout.columns - sizeX) / 2;
    }
    if ((align & ETextAlign::TA_VCENTER) != 0)
    {
        int sizeY = pFont->glyph->height;
        posY = (s_layout.rows - sizeY) / 2;
    }
    if ((align & ETextAlign::TA_MIDTEXT) != 0)
    {
        int len = text.length();
        int sizeX = CalculateGFXWidth(pFont, text.substr(0, len/2));
        if ((len % 2) != 0)
        {
            // Uneven characters, so add half of the width of the center character
            int extra = (CalculateGFXWidth(pFont, text.substr(len/2, 1)) / 2);
            sizeX += extra;
        }
        posX = (s_layout.columns / 2) - sizeX;
    }
    if ((align & ETextAlign::TA_END) != 0)
    {
        int sizeX = CalculateGFXWidth(pFont, text);
        posX = (s_layout.columns - sizeX) - x + 1;
    }

    for(char ch: text)
    {
        if (ch >= pFont->first && ch < pFont->last)
        {
            char c = (ch - pFont->first);

            GFXglyph *glyph = &(pFont->glyph[c]);
            uint8_t *bitmap = pFont->bitmap;

            uint16_t bo = glyph->bitmapOffset;
            uint8_t w = glyph->width;
            uint8_t h = glyph->height;

            uint8_t bits = 0;
            uint8_t bit = 0;
            for (int yy = h - 1; yy >= 0; yy--)
            {
                for (int xx = 0; xx < w; xx++)
                {
                    if (bit == 0x00)
                    {
                        bits = bitmap[bo++];
                        bit = 0x80;
                    }
                    if (bits & bit)
                    {
                        pLEDs[CalcLedPos(posX + xx, posY + y + (h - 1 - yy))] = color;
                    }
                    bit = bit >> 1;
                }
            }
            posX += glyph->xAdvance;
        }
    }
}


/// @brief Calculate the width of an text
/// @param font 
/// @param text 
int DisplayStateBase::CalculateGFXWidth(const GFXfont* font, std::string text)
{
    int16_t xo16 = 0;
    for(char ch: text)
    {
        if (ch >= font->first && ch < font->last)
        {
            char c = (ch - font->first);
            GFXglyph *glyph = &(font->glyph[c]);
            xo16 += glyph->xAdvance;
        }
    }
    return xo16;
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
        brightness = brightnessDay - ((epochTime - sunset) * brightnessDiff) / deltaTime;
    }    
    return brightness;
}
