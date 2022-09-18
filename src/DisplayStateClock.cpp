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
            // Can be used to check for special dates (not implemented anymore)
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

        // Set the background color (if required)
        FillBackground();

        // Determine the brightness
        int brightness = GetBrightness(epochTime);
        if (brightness != m_nPreviousBrightness)
        {
            log("Brightness changed to: %d", brightness);
            m_nPreviousBrightness = brightness;
        }    
  
        // Show birthdays / holidays
        if (m_fShowBirthday)
            AddWordToLeds(s_layout.extra.birthday, colDefault, brightness, EColorElement::CE_SPECIAL);
        if (m_fShowHoliday)
            AddWordToLeds(s_layout.extra.holiday, colDefault, brightness, EColorElement::CE_SPECIAL);

        // Always show it-is
        AddWordToLeds(pToPastWord, colDefault, brightness, EColorElement::CE_TIME); // (AddWordToLeds can handle NULL pointers!)
        AddWordToLeds(pMinutesMainWord, colDefault, brightness, EColorElement::CE_TIME);
        AddWordToLeds(pMinutesRestWord, colDefault, brightness, EColorElement::CE_TIME);
        AddWordToLeds(pHalfWord, colDefault, brightness, EColorElement::CE_TIME);
        AddWordToLeds(pHourWord, colDefault, brightness, EColorElement::CE_TIME);

        // Display day of the week
        AddWordToLeds(pDayWord, colDefault, brightness, EColorElement::CE_WEEKDAY);

        // Ad the date
        AddWordToLeds(pDayOfMonthWord, colDefault, brightness, EColorElement::CE_DATE);        
        AddWordToLeds(pMonthWord, colDefault, brightness, EColorElement::CE_DATE); 
        
        // Second heartbeat fading led
        AddWordToLeds(pTime->second, colDefault, brightness, EColorElement::CE_PULSE);
        
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
 
        case EColorElement::CE_UNKNOWN:
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



