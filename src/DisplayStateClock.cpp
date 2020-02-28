///////////////////////////////////////////////////////////////////////////////
// DisplayStateClock class
// Show the current time
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateClock.h"
#include <time.h>
#include <Timezone.h>  

// For now assume home
#define CLOCK_LATITUDE      52.306772
#define CLOCK_LONGITUDE     5.618550
#define CLOCK_ALTITUDE      10

#define YEAR_2000_TIMESTAMP     946684800ULL
#define SECONDS_PER_HOUR        3600ULL
#define SECONDS_PER_DAY         86400ULL

#define BRIGHTNESS_MAX          100
#define BRIGHTNESS_MIN          30
#define BRIGHTNESS_DIFF         (BRIGHTNESS_MAX - BRIGHTNESS_MIN)

//
// Initialize
//
void DisplayStateClock::Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings)
{
    DisplayStateBase::Initialize(pLEDs, pTZ, pSettings);
    m_nPreviousBrightness = 0;
    strcpy(m_sCommand, "clock");
    strcpy(m_sCommandDescription, "Show the normal clock");
}


//
// Calculate the sunrise or the sunset times for a specific date
// This is a first calculation with a aberration of max 3 minutes - for now it is OK. Otherwise, calcalution
// must be re-done with the last new time and put this value again in centuries_t value. 
// See documentation:  http://www.stargazing.net/kepler/sunrise.html
//
void DisplayStateClock::CalcSunriseSunset(unsigned long timestamp, float lat, float lon, float alt, unsigned long *sunrise, unsigned long *sunset)
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
void DisplayStateClock::UpdateBrightness(unsigned long epochTime)
{
    unsigned long sunrise, sunset;
    CalcSunriseSunset(epochTime, CLOCK_LATITUDE, CLOCK_LONGITUDE, CLOCK_ALTITUDE, &sunrise, &sunset);

    //           sunrise            sunset        
    //  |        |                  |        |
    //  |    __--|       100%       |--__    |
    //  |__--                            --__|   30%

    int brightness = BRIGHTNESS_MIN;
    unsigned long deltaTime = 30 * 60;
    if (epochTime > sunrise - deltaTime && epochTime <= sunrise)
    {
        // Morning Twilight
        brightness = BRIGHTNESS_MIN + ((sunrise - epochTime) * BRIGHTNESS_DIFF) / deltaTime;
    }
    else if (epochTime >= sunrise && epochTime <= sunset)
    {
        // Daytime
        brightness = BRIGHTNESS_MAX;
    }
    else if (epochTime >= sunset && epochTime < sunset + deltaTime)
    {
        // Evening Twilight
        brightness = BRIGHTNESS_MAX - ((epochTime - sunset) * BRIGHTNESS_DIFF) / deltaTime;
    }
    FastLED.setBrightness(brightness);
    if (brightness != m_nPreviousBrightness)
    {
        Serial.print("Brightness changed to: ");
        Serial.println(brightness);
        m_nPreviousBrightness = brightness;
    }    
}

//
// Loop
//
bool DisplayStateClock::HandleLoop(unsigned long epochTime)
{
    if (!m_pSettings) return false;

    if (Elapsed(m_timeStamp) > 100)
    {
        TimeChangeRule *tcr;    

        m_timeStamp = millis();

        //
        // Update the brightness
        UpdateBrightness(epochTime);        

        // Convert to local time
        time_t t = (m_pTZ != NULL) ? m_pTZ->toLocal(epochTime, &tcr) : 0; // (Note: tcr cannot be NULL)

        m_nWeekDay = weekday(t) - 1; // Weekday returns (1 - 7), Sunday = 1
        //int seconds = second(t);
        m_nHours = hour(t);
        m_nMinutes = minute(t);        
        int monthday = day(t);
        int monthnum = month(t) - 1; // Januari = 1, we need it to be 0

        // Quarter past 1 => 14 minutes to half two (in Dutch this is correct, English I don't know)
        int quarterNum = m_nMinutes / 15;
        int min1 = m_nMinutes % 15;        
        int hours = m_nHours;
        if (quarterNum > 0 && !(quarterNum == 1 && min1 == 0)) hours++; // Increase the hour, but do NOT increase when we are at exactly a quarter past
        hours %= 12; // Limit hours to 12
        
        // Determine to/past
        const ledpos_t* pMinutesWord = NULL;
        const ledpos_t* pHalfWord = NULL;
        const ledpos_t* pToPastWord = NULL;
        if (min1 == 0)
        {
            // Specials quarters:
            switch (quarterNum)             
            {
                case 0:                
                    pMinutesWord = hour_full;
                    pToPastWord = NULL; 
                    break;
                case 1:                
                    pMinutesWord = quarter;
                    pToPastWord = past; 
                    break;
                case 2:                
                    pMinutesWord = half;
                    pToPastWord = NULL; 
                    break;
                case 3:                
                    pMinutesWord = quarter;
                    pToPastWord = to; 
                    break;
            }
        }
        else
        {
            pToPastWord = past;
            if (quarterNum == 1 || quarterNum == 3)
            {
                min1 = 15 - min1;
                pToPastWord = to;
            }
            if (m_nMinutes > 15 && m_nMinutes < 45)
            {
                pHalfWord = half;
            }
            const ledpos_t* pMinuteWords[15] {NULL, minute_1, minute_2, minute_3, minute_4, minute_5, minute_6, minute_7, minute_8, minute_9, minute_10, minute_11, minute_12, minute_13, minute_14};
            pMinutesWord = pMinuteWords[min1 % 15];
        }

        const ledpos_t* pHourWord = NULL;
        const ledpos_t* pHourWords[12] {hour_12, hour_1, hour_2, hour_3, hour_4, hour_5, hour_6, hour_7, hour_8, hour_9, hour_10, hour_11};
        pHourWord = pHourWords[hours % 12];

        const ledpos_t* pDayWord = s_wordsWeekDays[m_nWeekDay % 7];
        const ledpos_t* pDayOfMonthWord = s_wordsMonthDays[(monthday - 1) % 31]; // Days start at 1
        const ledpos_t* pMonthWord = s_wordMonths[monthnum % 12];


        // And now for the LEDS
        FastLED.clear();

        // Always show it-is
        AddWordToLeds((ledpos_t*) itis, EColorElement::CE_ITIS);

        AddWordToLeds((ledpos_t*) pToPastWord, EColorElement::CE_TIME); // (AddWordToLeds can handle NULL pointers!)
        AddWordToLeds((ledpos_t*) pMinutesWord, EColorElement::CE_TIME);
        AddWordToLeds((ledpos_t*) pHalfWord, EColorElement::CE_TIME);
        AddWordToLeds((ledpos_t*) pHourWord, EColorElement::CE_TIME);

        // Display day of the week
        AddWordToLeds((ledpos_t*) pDayWord, EColorElement::CE_WEEKDAY);

        // Ad the date
        AddWordToLeds((ledpos_t*) pDayOfMonthWord, EColorElement::CE_DATE);        
        AddWordToLeds((ledpos_t*) pMonthWord, EColorElement::CE_DATE); 
        
        // Second heartbeat fading led
        AddWordToLeds((ledpos_t*) pulse2, EColorElement::CE_PULSE);
        
        FastLED.show();   
    }
    return true;
}


//
// Overridden from DisplayStateBase to provide the color for a word or character
//
CRGB DisplayStateClock::ColorHandler(uint32_t customParam)
{
    switch ((EColorElement) customParam)
    {
        case EColorElement::CE_ITIS:        
            return GetDisplayOptionsColor(m_pSettings->colTime, m_pSettings->eDisplayOptionsTime);

        case EColorElement::CE_TIME:
            return GetDisplayOptionsColor(m_pSettings->colTime, m_pSettings->eDisplayOptionsTime);

        case EColorElement::CE_WEEKDAY:
            return GetDisplayOptionsColor(m_pSettings->colWeekday, m_pSettings->eDisplayOptionsWeekday);

        case EColorElement::CE_DATE: 
            return GetDisplayOptionsColor(m_pSettings->colDate, m_pSettings->eDisplayOptionsDate);

        case EColorElement::CE_PULSE:
            return CRGB(m_pSettings->colDate).fadeToBlackBy(127 * (cos(2.0 * PI * m_timeStamp / 6000.0) + 1.0));

        default:
            return CRGB(customParam);
    }
}

CRGB DisplayStateClock::GetDisplayOptionsColor(CRGB defaultColor, MyriadclockSettings::EDisplayOptions eOption)
{
    static CRGB colorLoop[] {CRGB::Navy, CRGB::Blue, CRGB::Aqua, CRGB::Teal, CRGB::Olive, CRGB::Green, CRGB::Lime, CRGB::Yellow, 
                             CRGB::Orange, CRGB::Red, CRGB::Maroon, CRGB::Fuchsia, CRGB::Purple, CRGB::Magenta, CRGB::Silver, CRGB::White};

    // Weekday colors (according to the Ayurveda) : https://blog.forret.com/2007/08/21/weekday-colours-ayurveda/
    // Starting monday
    static CRGB colorWeeksAyurveda[] {CRGB::Maroon, CRGB::FloralWhite, CRGB::IndianRed, CRGB::LimeGreen, CRGB::LightYellow, CRGB::AliceBlue, CRGB(0x66, 0x33, 0x99)};
    // Thai
    static CRGB colorWeeksThai[] {CRGB::Red, CRGB::LightYellow, CRGB::Pink, CRGB::Green, CRGB::Grey, CRGB::Orange, CRGB::LightBlue, CRGB::Purple};

    switch (eOption)
    {
        case MyriadclockSettings::DO_COLOR_WEEK_AYURVEDA:
            return colorWeeksAyurveda[m_nWeekDay % 7];

        case MyriadclockSettings::DO_COLOR_WEEK_THAI:
            return colorWeeksThai[m_nWeekDay % 7];

        case MyriadclockSettings::DO_COLOR_CYCLENORMAL:
            return colorLoop[m_nMinutes % 16];
        
        case MyriadclockSettings::DO_COLOR_CYCLEHOUR:
            return colorLoop[m_nHours % 16];
        
        case MyriadclockSettings::DO_COLOR_PARTY_MINUTE:
            return colorLoop[m_nMinutes % 16]; // TODO

        default:
        case MyriadclockSettings::DO_NORMAL:
            return defaultColor;
    }    
}



