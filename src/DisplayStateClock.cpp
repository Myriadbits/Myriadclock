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
// Loop
//
bool DisplayStateClock::HandleLoop(unsigned long epochTime)
{
    if (!m_pSettings) return false;

    CRGB colTime = CRGB(m_pSettings->colTime);
    CRGB colDate = CRGB(m_pSettings->colDate);
    
    if (Elapsed(m_timeStamp) > 100)
    {
        TimeChangeRule *tcr;    

        m_timeStamp = millis();

        //
        // Auto adjust brightness depending upon sunrise/sunset
        //
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
        

        // And now for the LEDS
        FastLED.clear();

        // Convert to local time
        //TimeChangeRule *tcr;    
        time_t t = (m_pTZ != NULL) ? m_pTZ->toLocal(epochTime, &tcr) : 0; // (Note: tcr cannot be NULL)

        int dayofweek = weekday(t) - 1; // Weekday returns (1 - 7), Sunday = 1
        //int seconds = second(t);
        int hours = hour(t);
        int minutes = minute(t);        
        int monthday = day(t);
        int monthnum = month(t) - 1; // Januari = 1, we need it to be 0

        // Quarter past 1 => 14 minutes to half two (in Dutch this is correct, English I don't know)
        int quarterNum = minutes / 15;
        int min1 = minutes % 15;        
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
            if (minutes > 15 && minutes < 45)
            {
                pHalfWord = half;
            }
            switch (min1)
            {
                case  1: pMinutesWord = minute_1; break;            
                case  2: pMinutesWord = minute_2; break;
                case  3: pMinutesWord = minute_3; break;
                case  4: pMinutesWord = minute_4; break; 
                case  5: pMinutesWord = minute_5; break; 
                case  6: pMinutesWord = minute_6; break;
                case  7: pMinutesWord = minute_7; break; 
                case  8: pMinutesWord = minute_8; break; 
                case  9: pMinutesWord = minute_9; break; 
                case 10: pMinutesWord = minute_10; break; 
                case 11: pMinutesWord = minute_11; break;
                case 12: pMinutesWord = minute_12; break;
                case 13: pMinutesWord = minute_13; break;
                case 14: pMinutesWord = minute_14; break;
            }
        }
        AddWordToLeds((ledpos_t*) pToPastWord, colTime); // (AddWordToLeds can handle NULL pointers!)
        AddWordToLeds((ledpos_t*) pMinutesWord, colTime);
        AddWordToLeds((ledpos_t*) pHalfWord, colTime);

        // Display hour
        const ledpos_t* pHourWord = NULL;
        switch(hours)
        {
            case  0: pHourWord = hour_12; break;
            case  1: pHourWord = hour_1 ; break;
            case  2: pHourWord = hour_2 ; break;
            case  3: pHourWord = hour_3 ; break;
            case  4: pHourWord = hour_4 ; break;
            case  5: pHourWord = hour_5 ; break;
            case  6: pHourWord = hour_6 ; break;
            case  7: pHourWord = hour_7 ; break;
            case  8: pHourWord = hour_8 ; break;
            case  9: pHourWord = hour_9 ; break;
            case 10: pHourWord = hour_10; break;
            case 11: pHourWord = hour_11; break;
        }
        AddWordToLeds((ledpos_t*) pHourWord, colTime);

        // Display day of the week
        const ledpos_t* pDayWord = s_wordsWeekDays[dayofweek % 7];
        AddWordToLeds((ledpos_t*) pDayWord, colDate);
        
        // Display day of the month
        const ledpos_t* pDayOfMonthWord = s_wordsMonthDays[(monthday - 1) % 31]; // Days start at 1
        AddWordToLeds((ledpos_t*) pDayOfMonthWord, colDate);        

        // Display the month
        const ledpos_t* pMonthWord = s_wordMonths[monthnum % 12];
        AddWordToLeds((ledpos_t*) pMonthWord, colDate); 
        
        // Always show it-is
        AddWordToLeds((ledpos_t*) itis, colTime);

        // Second heartbeat fading led
        int heartBeatBrightness = 127 * (cos(2.0 * PI * m_timeStamp / 6000.0) + 1.0);
        CRGB colFade = colDate;
        colFade.fadeToBlackBy(heartBeatBrightness);
        AddWordToLeds((ledpos_t*) pulse2, colFade); // CRGB(0, heartBeatBrightness / 2, 0));        
        
        FastLED.show();   
    }
    return true;
}
