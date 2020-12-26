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

//
// Initialize
//
void DisplayStateClock::Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings)
{
    DisplayStateBase::Initialize(pLEDs, pTZ, pSettings);
    m_nPreviousBrightness = 0;
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

    int brightness = m_pSettings->nBrightnessNight;
    unsigned long deltaTime = 30 * 60;
    int brightnessDiff = (m_pSettings->nBrightnessDay -  m_pSettings->nBrightnessNight);
    if (epochTime > sunrise - deltaTime && epochTime <= sunrise)
    {
        // Morning Twilight
        brightness =  m_pSettings->nBrightnessNight + ((sunrise - epochTime) * brightnessDiff) / deltaTime;
    }
    else if (epochTime >= sunrise && epochTime <= sunset)
    {
        // Daytime
        brightness = m_pSettings->nBrightnessDay;
    }
    else if (epochTime >= sunset && epochTime < sunset + deltaTime)
    {
        // Evening Twilight
        brightness = m_pSettings->nBrightnessDay - ((epochTime - sunset) * brightnessDiff) / deltaTime;
    }
    FastLED.setBrightness(brightness);
    if (brightness != m_nPreviousBrightness)
    {
        log("Brightness changed to: %d", brightness);
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

        // Convert to local time
        time_t t = (m_pTZ != NULL) ? m_pTZ->toLocal(epochTime, &tcr) : 0; // (Note: tcr cannot be NULL)

        CRGB colDefault = m_pSettings->colTime;

        m_nWeekDay = weekday(t) - 1; // Weekday returns (1 - 7), Sunday = 1
        m_nSeconds = second(t);
        m_nHours = hour(t);
        m_nMinutes = minute(t);        
        int monthday = day(t);
        int monthnum = month(t) - 1; // Januari = 1, we need it to be 0

        switch (m_pSettings->eDisplayOptionsTime)
        {
        case MyriadclockSettings::DO_COLOR_PARTY_QUICK:
            m_randomTime.seed(m_nHours + m_nMinutes + m_nSeconds);
            break;
        case MyriadclockSettings::DO_COLOR_PARTY_MINUTE:
            m_randomTime.seed(m_nHours + m_nMinutes);
            break;
        case MyriadclockSettings::DO_COLOR_PARTY_SLOW:
            m_randomTime.seed(m_nHours + m_nMinutes + (m_nSeconds / 10));
            break;
        default:
            break;
        }

        switch (m_pSettings->eDisplayOptionsWeekday)
        {
        case MyriadclockSettings::DO_COLOR_PARTY_QUICK:
            m_randomWeekday.seed(m_nHours + m_nMinutes + m_nSeconds);
            break;
        case MyriadclockSettings::DO_COLOR_PARTY_MINUTE:
            m_randomWeekday.seed(m_nHours + m_nMinutes);
            break;
        case MyriadclockSettings::DO_COLOR_PARTY_SLOW:
            m_randomWeekday.seed(m_nHours + m_nMinutes + (m_nSeconds / 10));
            break;
        default:
            break;
        }

        switch (m_pSettings->eDisplayOptionsDate)
        {
        case MyriadclockSettings::DO_COLOR_PARTY_QUICK:
            m_randomDate.seed(m_nHours + m_nMinutes + m_nSeconds);
            break;
        case MyriadclockSettings::DO_COLOR_PARTY_MINUTE:
            m_randomDate.seed(m_nHours + m_nMinutes);
            break;
        case MyriadclockSettings::DO_COLOR_PARTY_SLOW:
            m_randomDate.seed(m_nHours + m_nMinutes + (m_nSeconds / 10));
            break;
        default:
            break;
        }

        // Quarter past 1 => 14 minutes to half two (in Dutch this is correct, English I don't know)
        int min5 = m_nMinutes / 5;
        int min1 = m_nMinutes % 5;        
        int hours = m_nHours;
        if (min5 > 3) hours++; // Increase the hour, but do NOT increase when we are at exactly a quarter past
        hours %= 12; // Limit hours to 12

        const ledpos_t* pMinutesMainWord = NULL;
        const ledpos_t* pMinutesRestWord = NULL;
        const ledpos_t* pHalfWord = NULL;
        const ledpos_t* pToPastWord = NULL;

        const ledtime_t* pTime = &(s_layout.time);

        switch (s_layout.timeFormat)
        {
        case ETimeFormat::TF_NL_EVERYMIN:
            {
                pMinutesMainWord = pTime->leadtext;

                // Quarter past 1 => 14 minutes to half two (in Dutch this is correct, English I don't know)
                int quarterNum = m_nMinutes / 15;
                int min1 = m_nMinutes % 15;        
                int hours = m_nHours;
                if (quarterNum > 0 && !(quarterNum == 1 && min1 == 0)) hours++; // Increase the hour, but do NOT increase when we are at exactly a quarter past
                hours %= 12; // Limit hours to 12
                
                // Determine to/past
                if (min1 == 0)
                {
                    // Specials quarters:
                    switch (quarterNum)             
                    {
                        case 0:                
                            pMinutesRestWord = pTime->hour_full;
                            pToPastWord = NULL; 
                            break;
                        case 1:                
                            pMinutesRestWord = pTime->quarter;
                            pToPastWord = pTime->past_15; 
                            break;
                        case 2:                
                            pMinutesRestWord = pTime->half_to;
                            pToPastWord = NULL; 
                            break;
                        case 3:                
                            pMinutesRestWord = pTime->quarter;
                            pToPastWord = pTime->to_15; 
                            break;
                    }
                }
                else
                {
                    pToPastWord = pTime->past_5;
                    if (quarterNum == 1 || quarterNum == 3)
                    {
                        min1 = 15 - min1;
                        pToPastWord = pTime->to_5;
                    }
                    if (m_nMinutes > 15 && m_nMinutes < 45)
                    {
                        pHalfWord =pTime-> half_to;
                    }
                    const ledpos_t* pMinuteWords[15] {NULL,pTime->minute_1, pTime->minute_2, pTime->minute_3, pTime->minute_4, 
                        pTime->minute_5, pTime->minute_6, pTime->minute_7, pTime->minute_8, pTime->minute_10, 
                        pTime->minute_11, pTime->minute_12, pTime->minute_13, pTime->minute_14};
                    pMinutesRestWord = pMinuteWords[min1 % 15];
                }               
            }
            break;
        
        case ETimeFormat::TF_NL_5MIN:
            {
                switch(min5)
                {
                    case  0: pMinutesMainWord = pTime->hour_full; break;
                    case  1: pMinutesMainWord = pTime->minute_5 ; pToPastWord = pTime->past_5 ; break;
                    case  2: pMinutesMainWord = pTime->minute_10; pToPastWord = pTime->past_10; break;
                    case  3: pMinutesMainWord = pTime->quarter;   pToPastWord = pTime->past_15; break;
                    case  4: pMinutesMainWord = pTime->minute_10; pToPastWord = pTime->to_10  ; pHalfWord = pTime->half_to; break;
                    case  5: pMinutesMainWord = pTime->minute_5 ; pToPastWord = pTime->to_5   ; pHalfWord = pTime->half_to; break;
                    case  6: pMinutesMainWord = pTime->half_to; break;
                    case  7: pMinutesMainWord = pTime->minute_5 ; pToPastWord = pTime->past_5 ; pHalfWord = pTime->half_past; break;
                    case  8: pMinutesMainWord = pTime->minute_10; pToPastWord = pTime->past_10; pHalfWord = pTime->half_past; break;
                    case  9: pMinutesMainWord = pTime->quarter;   pToPastWord = pTime->to_15  ; break;
                    case 10: pMinutesMainWord = pTime->minute_10; pToPastWord = pTime->to_10  ; break;
                    case 11: pMinutesMainWord = pTime->minute_5 ; pToPastWord = pTime->to_5   ; break;
                }
                
                const ledpos_t* pMinute5Words[5] {NULL, pTime->minute_1, pTime->minute_2, pTime->minute_3,pTime->minute_4};
                pMinutesRestWord = pMinute5Words[min1];
            }
            break;

        case ETimeFormat::TF_EN_5MIN:
        default:
            {

            }
            break;
        }

        const ledpos_t* pHourWord = NULL;
        pHourWord = s_layout.hours[hours % 12];

        const ledpos_t* pDayWord = s_layout.weekdays[m_nWeekDay % 7];
        const ledpos_t* pDayOfMonthWord = s_layout.days[(monthday - 1) % 31]; // Days start at 1
        const ledpos_t* pMonthWord = s_layout.months[monthnum % 12];

        // And now for the LEDS
        FastLED.clear();

        // Back to full brightness
        FastLED.setBrightness(255);

        // Set the background color (if required)
        FillBackground();

        // Update the brightness
        UpdateBrightness(epochTime);        

        // Always show it-is
        AddWordToLeds((ledpos_t*) pToPastWord, colDefault, EColorElement::CE_TIME); // (AddWordToLeds can handle NULL pointers!)
        AddWordToLeds((ledpos_t*) pMinutesMainWord, colDefault, EColorElement::CE_TIME);
        AddWordToLeds((ledpos_t*) pMinutesRestWord, colDefault, EColorElement::CE_TIME);
        AddWordToLeds((ledpos_t*) pHalfWord, colDefault, EColorElement::CE_TIME);
        AddWordToLeds((ledpos_t*) pHourWord, colDefault, EColorElement::CE_TIME);

        // Display day of the week
        AddWordToLeds((ledpos_t*) pDayWord, colDefault, EColorElement::CE_WEEKDAY);

        // Ad the date
        AddWordToLeds((ledpos_t*) pDayOfMonthWord, colDefault, EColorElement::CE_DATE);        
        AddWordToLeds((ledpos_t*) pMonthWord, colDefault, EColorElement::CE_DATE); 
        
        // Second heartbeat fading led
        AddWordToLeds((ledpos_t*) pTime->second, colDefault, EColorElement::CE_PULSE);
        
        FastLED.show();   
    }
    return true;
}


//
// Overridden from DisplayStateBase to provide the color for a word or character
//
CRGB DisplayStateClock::ColorHandler(CRGB defaultColor, int customParam)
{
    switch ((EColorElement) customParam)
    {
        case EColorElement::CE_ITIS:        
            return GetDisplayOptionsColor(m_pSettings->colTime, m_pSettings->eDisplayOptionsTime, m_randomTime);

        case EColorElement::CE_TIME:
            return GetDisplayOptionsColor(m_pSettings->colTime, m_pSettings->eDisplayOptionsTime, m_randomTime);

        case EColorElement::CE_WEEKDAY:
            return GetDisplayOptionsColor(m_pSettings->colWeekday, m_pSettings->eDisplayOptionsWeekday, m_randomWeekday);

        case EColorElement::CE_DATE: 
            return GetDisplayOptionsColor(m_pSettings->colDate, m_pSettings->eDisplayOptionsDate, m_randomDate);

        case EColorElement::CE_PULSE:
            return CRGB(m_pSettings->colDate).fadeToBlackBy(127 * (cos(2.0 * PI * m_timeStamp / 6000.0) + 1.0));

        default:
            return defaultColor;
    }
}

//
// Helper to return a specific color for a display part
//
CRGB DisplayStateClock::GetDisplayOptionsColor(CRGB defaultColor, MyriadclockSettings::EDisplayOptions eOption, std::minstd_rand0& generator)
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
        
        case MyriadclockSettings::DO_COLOR_PARTY_QUICK:
        case MyriadclockSettings::DO_COLOR_PARTY_MINUTE:
        case MyriadclockSettings::DO_COLOR_PARTY_SLOW:
            return colorLoop[generator() % 16];

        default:
        case MyriadclockSettings::DO_NORMAL:
            return defaultColor;
    }    
}



