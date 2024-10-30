///////////////////////////////////////////////////////////////////////////////
// DisplayStateClock class
// Show the current time
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateClock.h"
#include <time.h>
#include <Timezone.h>  
#include "Console.h"
#include "DisplayStateManager.h"
#include "FontDrawer.h"

using namespace std;

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
    m_nTransitionStep = 0;
    m_nPreviousMinute = 0;
    m_nPreviousBrightness = 0;
    m_fMinutesMainInTrans = false;
    m_fMinutesRestInTrans = false;
    m_fToPastInTrans = false;
    m_fHalfWordInTrans = false;
    m_fHourWordInTrans = false;

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

    const int refreshTime = 100;
    if (Elapsed(m_timeStamp) > refreshTime)
    {
        m_timeStamp = millis();

        // Convert to local time
        time_t t = localTime;

        CRGB colDefault = m_pConfig->getConfigValue(CONFIG_COLOR_TIME);
        int brightnessBackground = m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_BACKGROUND);

        // Note these vars are members since they are used in multiple functions
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

        // Once every minute, check birthdays
        if (m_nPreviousMinute != m_nMinutes)
        {
            // Can be used to check for special dates (not implemented anymore)
            m_nPreviousMinute = m_nMinutes;            
        }
        bool fShowHeart = (monthnum == 1 && monthday == 14);

        // TRANSITIONS

        // Get the clock words from the convertor
        // The return struct can contain nullptr, those words do not need to be drawn/lit
        ClockTimeWordConvertor::convert(t, &s_layout, &m_sClockWordsNow);

        // Determine the brightness
        int brightness = GetBrightness(epochTime);
        if (brightness != m_nPreviousBrightness)
        {
            log("Brightness changed to: %d", brightness);
            m_nPreviousBrightness = brightness;
        }

        // TRANSITIONS
        // Currently only a fade out fade in transition
        // TODO: Make different transitions (words moving transition, incoming letters transition)
        int transitionTime = 4; // Time in seconds
        int brightnessTransition = brightness;
        if (m_nTransitionStep == 0)
        {
            // Waiting to start a transition
            if (m_nSeconds >= 60 - (transitionTime / 2))
            {
                // Time to start a transition!

                // Determine what will transition
                ClockTimeWordConvertor::convert(localTime + transitionTime, &s_layout, &m_sClockWordsNext);
                m_fToPastInTrans = (m_sClockWordsNow.pToPastWord != m_sClockWordsNext.pToPastWord);
                m_fMinutesMainInTrans = (m_sClockWordsNow.pMinutesMainWord != m_sClockWordsNext.pMinutesMainWord);
                m_fMinutesRestInTrans = (m_sClockWordsNow.pMinutesRestWord != m_sClockWordsNext.pMinutesRestWord);
                m_fHalfWordInTrans = (m_sClockWordsNow.pHalfWord != m_sClockWordsNext.pHalfWord);
                m_fHourWordInTrans = (m_sClockWordsNow.pHourWord != m_sClockWordsNext.pHourWord);

                // Start the transition
                m_nTransitionStep = 1;
            }
        }
        else                   
        {
            // Transition time
            int transitionSteps = transitionTime * 1000 / refreshTime;
            int transitionHalfSteps = transitionSteps / 2;
            float_t brightnessStep = (float_t)(brightnessTransition - brightnessBackground) / transitionHalfSteps;
            if (m_nTransitionStep < transitionHalfSteps)
            {
                brightnessTransition = brightness - brightnessStep * m_nTransitionStep;
                m_nTransitionStep++;
            }
            else if (m_nTransitionStep < transitionSteps)
            {
                brightnessTransition = brightnessBackground + brightnessStep * (m_nTransitionStep - transitionHalfSteps);
                m_nTransitionStep++;
            }
            else
            {
                // Stop the transition
                m_nTransitionStep = 0;
            }
        }

        // DRAWING

        // And now for the LEDS
        FastLED.clear();

        // Set the background color (if required)
        FillBackground(brightnessBackground);

        if (m_pManager != nullptr && m_pManager->getIsCloxel())
        {
            int option = 0;
            char buff[16];

            int cloxelOptions = 0;
            if (m_pConfig != NULL) 
            {
                cloxelOptions = m_pConfig->getConfigValue(CONFIG_OPTIONS_CLOXEL);
            }

            const char* daysOfWeek[] = { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };
            string weekdayText = string(daysOfWeek[m_nWeekDay % 7]);

            if (cloxelOptions == 0)
            {
                snprintf(buff, sizeof(buff), "%s %02d:%02d", weekdayText.c_str(), m_nHours, m_nMinutes);
                std::string text = buff;
                // TODO use the colorhandlers in drawing the text!
                FontDrawer::getInstance().DrawGFX(m_pLEDs, EFontType::FT_HIGHFONT48, ETextAlign::TA_MIDTEXT | ETextAlign::TA_VCENTER, 0, 0, text, colDefault, brightness);
            }
            else if (cloxelOptions == 1)
            {
                snprintf(buff, sizeof(buff), "%s %02d:%02d", weekdayText.c_str(), m_nHours, m_nMinutes);
                std::string text = buff;
                FontDrawer::getInstance().DrawGFX(m_pLEDs, EFontType::FT_HIGHFONT46, ETextAlign::TA_MIDTEXT | ETextAlign::TA_VCENTER, 0, 0, text, colDefault, brightness);
            }
            else if (cloxelOptions == 2)
            {
                snprintf(buff, sizeof(buff), "%02d:%02d:%02d", m_nHours, m_nMinutes, m_nSeconds);
                std::string text = buff;
                FontDrawer::getInstance().DrawGFX(m_pLEDs, EFontType::FT_HIGHFONT46, ETextAlign::TA_MIDTEXT | ETextAlign::TA_VCENTER, 0, 0, text, colDefault, brightness);
            }
            else if (cloxelOptions == 3)
            {
                snprintf(buff, sizeof(buff), "%02d:%02d", m_nHours, m_nMinutes);
                std::string text = buff;
                FontDrawer::getInstance().DrawGFX(m_pLEDs, EFontType::FT_HIGHFONT46, ETextAlign::TA_MIDTEXT | ETextAlign::TA_VCENTER, 0, 0, text, colDefault, brightness);
                CRGB colSeconds = ColorHandler(colDefault, brightness, EColorElement::CE_PULSE);
                if (m_nSeconds >= 0 && m_nSeconds <= 12)
                    m_pLEDs[CloxelLedPos(16 + m_nSeconds - 1, 0)] = colSeconds;
                else if (m_nSeconds > 12 && m_nSeconds <= 19)
                    m_pLEDs[CloxelLedPos(16 + 11, 1 + (m_nSeconds - 13))] = colSeconds;
                else if (m_nSeconds > 19 && m_nSeconds <= 42)
                    m_pLEDs[CloxelLedPos(16 + 10 - (m_nSeconds - 20), 7)] = colSeconds;
                else if (m_nSeconds > 42 && m_nSeconds <= 49)
                    m_pLEDs[CloxelLedPos(16 - 12, 6 - (m_nSeconds - 43))] = colSeconds;
                else if (m_nSeconds > 49 && m_nSeconds < 60)
                    m_pLEDs[CloxelLedPos(16 - 12 + (m_nSeconds - 49), 0)] = colSeconds;
            }
        }
        else
        {  
            // Show birthdays / holidays
            if (m_fShowBirthday)
                AddWordToLeds(s_layout.extra.birthday, colDefault, brightness, EColorElement::CE_SPECIAL);
            if (m_fShowHoliday)
                AddWordToLeds(s_layout.extra.holiday, colDefault, brightness, EColorElement::CE_SPECIAL);
            if (fShowHeart)
                AddWordToLeds(s_layout.extra.heart, CRGB::Red, brightness, EColorElement::CE_UNKNOWN);

            // Show/draw all Time related stuff
            AddWordToLeds(m_sClockWordsNow.pToPastWord, colDefault, (m_fToPastInTrans) ? brightnessTransition : brightness, EColorElement::CE_TIME); // Note that AddWordToLeds can handle NULL pointers!

            AddWordToLeds(m_sClockWordsNow.pMinutesMainWord, colDefault, (m_fMinutesMainInTrans) ? brightnessTransition : brightness, EColorElement::CE_TIME);
            AddWordToLeds(m_sClockWordsNow.pMinutesRestWord, colDefault, (m_fMinutesRestInTrans) ? brightnessTransition : brightness, EColorElement::CE_TIME);
            AddWordToLeds(m_sClockWordsNow.pHalfWord, colDefault, (m_fHalfWordInTrans) ? brightnessTransition : brightness, EColorElement::CE_TIME);
            AddWordToLeds(m_sClockWordsNow.pHourWord, colDefault, (m_fHourWordInTrans) ? brightnessTransition : brightness, EColorElement::CE_TIME);

            // Display day of the week
            AddWordToLeds(m_sClockWordsNow.pDayWord, colDefault, brightness, EColorElement::CE_WEEKDAY);

            // Add the date
            AddWordToLeds(m_sClockWordsNow.pDayOfMonthWord, colDefault, brightness, EColorElement::CE_DATE);        
            AddWordToLeds(m_sClockWordsNow.pMonthWord, colDefault, brightness, EColorElement::CE_DATE); 
            
            // Second heartbeat fading led
            AddWordToLeds(m_sClockWordsNow.pSecondLeds, colDefault, brightness, EColorElement::CE_PULSE);
        }            
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



