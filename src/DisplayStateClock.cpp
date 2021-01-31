///////////////////////////////////////////////////////////////////////////////
// DisplayStateClock class
// Show the current time
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateClock.h"
#include <time.h>
#include <Timezone.h>  
#include "Console.h"

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
void DisplayStateClock::Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager)
{
    DisplayStateBase::Initialize(pLEDs, pConfig, pManager);
    m_nPreviousBrightness = 0;

    Console::getInstance().add("backlight", this, "Set the intensity of the backlight");
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

    int brightnessDay = m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_NIGHT);
    int brightnessNight = m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_NIGHT);


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
    if (brightness != m_nPreviousBrightness)
    {
        log("Brightness changed to: %d", brightness);
        m_nPreviousBrightness = brightness;
        m_nBrightness = brightness;
    }    
}

//
// Check all special dates
void DisplayStateClock::CheckSpecialDates(const int monthday, const int monthnum)
{
    // Check all birthdays
    m_fShowBirthday = false;

    static int birthdayIndices[] {CONFIG_BIRTHDAY_1, CONFIG_BIRTHDAY_2, CONFIG_BIRTHDAY_3, CONFIG_BIRTHDAY_4};

    for(int n = 0; n < 4; n++)
    {
        BLEConfigItemDate* pdate = (BLEConfigItemDate*) m_pConfig->getConfigItem(birthdayIndices[n]);
        if (pdate != NULL)
        {
            if (pdate->getDay() == monthday && pdate->getMonth() == monthnum)
            {
                // It is a birthday, show the birthday text
                m_fShowBirthday = true;
                break; // No need to check the rest
            }
        }
    }       

    // // Check all holidays
    // m_fShowHoliday = false;
    // for(int n = 0; n < MAX_HOLIDAYS; n++)
    // {
    //     if ( (((m_pSettings->dateHolidays[n] & 0xFF000000) >> 24) == monthday) && 
    //         (((m_pSettings->dateHolidays[n] & 0x00FF0000) >> 16) == monthnum) )
    //     {
    //         m_fShowHoliday = true;
    //         break; // No need to check the rest
    //     }
    // }   
}

//
// Get a seed for a specific display option
int DisplayStateClock::GetSeed(EDisplayOptions eOption)
{
    switch (eOption)
    {
    case DO_COLOR_PARTY_QUICK:
        return m_nHours + m_nMinutes + m_nSeconds;
    case DO_COLOR_PARTY_MINUTE:
        return m_nHours + m_nMinutes;
        break;
    case DO_COLOR_PARTY_SLOW:
        return m_nHours + m_nMinutes + (m_nSeconds / 10);
    default:
        break;
    }
    return 0;
}

//
// Loop
//
bool DisplayStateClock::HandleLoop(unsigned long epochTime, time_t localTime)
{
    if (!m_pConfig) return false;

    if (Elapsed(m_timeStamp) > 100)
    {
        m_timeStamp = millis();

        // Convert to local time
        time_t t = localTime;

        CRGB colDefault = m_pConfig->getConfigValue(CONFIG_COLOR_TIME);

        m_nWeekDay = weekday(t) - 1; // Weekday returns (1 - 7), Sunday = 1
        m_nSeconds = second(t);
        m_nHours = hour(t);
        m_nMinutes = minute(t);        
        int monthday = day(t);
        int monthnum = month(t) - 1; // Januari = 1, we need it to be 0

        // Get the seed for a specific minute/hour/second
        m_randomTime.seed(GetSeed((EDisplayOptions) m_pConfig->getConfigValue(CONFIG_OPTIONS_TIME)));
        m_randomWeekday.seed(GetSeed((EDisplayOptions) m_pConfig->getConfigValue(CONFIG_OPTIONS_WEEKDAY)));
        m_randomDate.seed(GetSeed((EDisplayOptions) m_pConfig->getConfigValue(CONFIG_OPTIONS_DATE)));
        m_randomSpecial.seed(GetSeed((EDisplayOptions) m_pConfig->getConfigValue(CONFIG_OPTIONS_SPECIAL)));      

        // Quarter past 1 => 14 minutes to half two (in Dutch this is correct, English I don't know)
        int min5 = m_nMinutes / 5;
        int min1 = m_nMinutes % 5;        
        int hours = m_nHours;

        const ledpos_t* pMinutesMainWord = NULL;
        const ledpos_t* pMinutesRestWord = NULL;
        const ledpos_t* pHalfWord = NULL;
        const ledpos_t* pToPastWord = NULL;

        const ledtime_t* pTime = &(s_layout.time);

        // Once every minute, check birthdays
        if (m_nPreviousMinute != m_nMinutes)
        {
            CheckSpecialDates(monthday, monthnum + 1);
            m_nPreviousMinute = m_nMinutes;
        }

        switch (s_layout.timeFormat)
        {
        case ETimeFormat::TF_NL_EVERYMIN:
            {
                pMinutesMainWord = pTime->leadtext;

                if (min5 > 3) hours++; // Increase the hour, but do NOT increase when we are at exactly a quarter past
                hours %= 12; // Limit hours to 12

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
                if (min5 > 3) hours++; // Increase the hour, but do NOT increase when we are at exactly a quarter past
                hours %= 12; // Limit hours to 12

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
                if (min5 > 6) hours++; // Increase the hour, when after the half
                hours %= 12; // Limit hours to 12

                switch(min5)
                {
                    case  0: pMinutesMainWord = pTime->hour_full; break;
                    case  1: pMinutesMainWord = pTime->minute_5 ; pToPastWord = pTime->past_5 ; break;
                    case  2: pMinutesMainWord = pTime->minute_10; pToPastWord = pTime->past_10; break;
                    case  3: pMinutesMainWord = pTime->quarter;   pToPastWord = pTime->past_15; break;
                    case  4: pMinutesMainWord = pTime->minute_20; pToPastWord = pTime->past_20; break;
                    case  5: pMinutesMainWord = pTime->minute_25; pToPastWord = pTime->past_25; break;
                    case  6: pMinutesMainWord = pTime->half_past; break;
                    case  7: pMinutesMainWord = pTime->minute_25; pToPastWord = pTime->to_25; break;
                    case  8: pMinutesMainWord = pTime->minute_20; pToPastWord = pTime->to_20; break;
                    case  9: pMinutesMainWord = pTime->quarter;   pToPastWord = pTime->to_15; break;
                    case 10: pMinutesMainWord = pTime->minute_10; pToPastWord = pTime->to_10; break;
                    case 11: pMinutesMainWord = pTime->minute_5 ; pToPastWord = pTime->to_5 ; break;
                }
                
                const ledpos_t* pMinute5Words[5] {NULL, pTime->minute_1, pTime->minute_2, pTime->minute_3,pTime->minute_4};
                pMinutesRestWord = pMinute5Words[min1];
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

        // Show birthdays / holidays
        if (m_fShowBirthday)
            AddWordToLeds(s_layout.extra.birthday, colDefault, m_nBrightness, EColorElement::CE_SPECIAL);
        if (m_fShowHoliday)
            AddWordToLeds(s_layout.extra.holiday, colDefault, m_nBrightness, EColorElement::CE_SPECIAL);

        // Always show it-is
        AddWordToLeds(pToPastWord, colDefault, m_nBrightness, EColorElement::CE_TIME); // (AddWordToLeds can handle NULL pointers!)
        AddWordToLeds(pMinutesMainWord, colDefault, m_nBrightness, EColorElement::CE_TIME);
        AddWordToLeds(pMinutesRestWord, colDefault, m_nBrightness, EColorElement::CE_TIME);
        AddWordToLeds(pHalfWord, colDefault, m_nBrightness, EColorElement::CE_TIME);
        AddWordToLeds(pHourWord, colDefault, m_nBrightness, EColorElement::CE_TIME);

        // Display day of the week
        AddWordToLeds(pDayWord, colDefault, m_nBrightness, EColorElement::CE_WEEKDAY);

        // Ad the date
        AddWordToLeds(pDayOfMonthWord, colDefault, m_nBrightness, EColorElement::CE_DATE);        
        AddWordToLeds(pMonthWord, colDefault, m_nBrightness, EColorElement::CE_DATE); 
        
        // Second heartbeat fading led
        AddWordToLeds(pTime->second, colDefault, m_nBrightness, EColorElement::CE_PULSE);
        
        FastLED.show();   
    }
    return true;
}


//
// Overridden from DisplayStateBase to provide the color for a word or character
//
CRGB DisplayStateClock::ColorHandler(CRGB defaultColor, int brightness, int customParam)
{
    CRGB colRet = CRGB();
    switch ((EColorElement) customParam)
    {
        case EColorElement::CE_ITIS:        
        case EColorElement::CE_TIME:
            colRet = GetDisplayOptionsColor(CONFIG_COLOR_TIME, CONFIG_OPTIONS_TIME, m_randomTime);
            break;

        case EColorElement::CE_WEEKDAY:
            colRet = GetDisplayOptionsColor(CONFIG_COLOR_WEEKDAY, CONFIG_OPTIONS_WEEKDAY, m_randomWeekday);
            break;

        case EColorElement::CE_DATE: 
            colRet = GetDisplayOptionsColor(CONFIG_COLOR_DATE, CONFIG_OPTIONS_DATE, m_randomDate);
            break;

        case EColorElement::CE_PULSE:
            colRet = CRGB(m_pConfig->getConfigValue(CONFIG_COLOR_DATE)).fadeToBlackBy(127 * (cos(2.0 * PI * m_timeStamp / 6000.0) + 1.0));
            break;
 
        case EColorElement::CE_SPECIAL:
            colRet = GetDisplayOptionsColor(CONFIG_COLOR_DATE, CONFIG_OPTIONS_SPECIAL, m_randomSpecial);
            break;

        default:
            colRet = defaultColor;
            break;
    }
    return colRet.fadeLightBy(255 - brightness);
}

//
// Helper to return a specific color for a display part
//
CRGB DisplayStateClock::GetDisplayOptionsColor(int colorIndex, int optionIndex, std::minstd_rand0& generator)
{    
    static CRGB colorLoop[] {CRGB::Navy, CRGB::LightBlue, CRGB::Aqua, CRGB::Teal, CRGB::Olive, CRGB::Green, CRGB::Lime, CRGB::Yellow, 
                             CRGB::Orange, CRGB::Red, CRGB::Maroon, CRGB::Fuchsia, CRGB::Purple, CRGB::Magenta, CRGB::Silver, CRGB::White};

    CRGB defaultColor = m_pConfig->getConfigValue(colorIndex);
    EDisplayOptions eOption = (EDisplayOptions) m_pConfig->getConfigValue(optionIndex);

    // Weekday colors (according to the Ayurveda) : https://blog.forret.com/2007/08/21/weekday-colours-ayurveda/
    // Starting monday
    static CRGB colorWeeksAyurveda[] {CRGB::Maroon, CRGB::FloralWhite, CRGB::IndianRed, CRGB::LimeGreen, CRGB::LightYellow, CRGB::AliceBlue, CRGB(0x66, 0x33, 0x99)};
    // Thai
    static CRGB colorWeeksThai[] {CRGB::Red, CRGB::LightYellow, CRGB::Pink, CRGB::Green, CRGB::Grey, CRGB::Orange, CRGB::LightBlue, CRGB::Purple};

    switch (eOption)
    {
        case DO_COLOR_WEEK_AYURVEDA:
            return colorWeeksAyurveda[m_nWeekDay % 7];

        case DO_COLOR_WEEK_THAI:
            return colorWeeksThai[m_nWeekDay % 7];

        case DO_COLOR_CYCLENORMAL:
            return colorLoop[m_nMinutes % 16];
        
        case DO_COLOR_CYCLEHOUR:
            return colorLoop[m_nHours % 16];
        
        case DO_COLOR_PARTY_QUICK:
        case DO_COLOR_PARTY_MINUTE:
        case DO_COLOR_PARTY_SLOW:
            return colorLoop[generator() % 16];

        default:
        case DO_STATIC:
            return defaultColor;
    }    
}

//
// A command is called
//
void DisplayStateClock::commandHandler(std::string command, std::vector<std::string> args)
{
    if (command == "backlight" && args.size() > 0)
    {
        int value = atoi(args[1].c_str());
        if (m_pConfig != NULL)
        {
            BLEConfigItemUInt32* pItem = (BLEConfigItemUInt32*) m_pConfig->getConfigItem(CONFIG_BRIGHTNESS_BACKGROUND);
            if (pItem != NULL)
                pItem->setValue(value); // Change the brightness
            m_pConfig->store(); // And store
        }       
    }
}



