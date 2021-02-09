#pragma once

#include "FastLED.h"            // Fastled library to control the LEDs
#include "Console.h"
#include "ClockLayoutNL_V1.h"
#include "ClockLayoutNL_V2.h"
#include <Timezone.h>
#include "../lib/BLEConfig/include/BLEConfig.h"

#define MAXCOMMANDLEN               16
#define MAXCOMMANDDESCRIPTIONLEN    80

#define MAX_BIRTHDAYS       10
#define MAX_HOLIDAYS        10

//
// All configuration items
#define CONFIG_WIFI                     1
#define CONFIG_NAME                     2
#define CONFIG_LAYOUT                   3
#define CONFIG_TIMEZONE                 5
#define CONFIG_DAYLIGHTSAVING           6
#define CONFIG_COLOR_TIME               10
#define CONFIG_COLOR_WEEKDAY            11
#define CONFIG_COLOR_DATE               12
#define CONFIG_BRIGHTNESS_DAY           13
#define CONFIG_BRIGHTNESS_NIGHT         14
#define CONFIG_BRIGHTNESS_BACKGROUND    15

#define CONFIG_OPTIONS_TIME             20
#define CONFIG_OPTIONS_WEEKDAY          21
#define CONFIG_OPTIONS_DATE             22
#define CONFIG_OPTIONS_SPECIAL          23

#define CONFIG_BIRTHDAY_1               30
#define CONFIG_BIRTHDAY_2               31
#define CONFIG_BIRTHDAY_3               32
#define CONFIG_BIRTHDAY_4               33


enum EDisplayOptions
{
    DO_STATIC,              // Static color
    DO_COLOR_CYCLENORMAL,   // Normal color cycle
    DO_COLOR_CYCLEHOUR,     // Color cycle per hour
    DO_COLOR_CYCLERESERVED, // <reserved>
    DO_COLOR_PARTY_SLOW,    // Random per character per minute
    DO_COLOR_PARTY_QUICK,   // Random per character constant
    DO_COLOR_PARTY_MINUTE,  // Random colors every minute for a few seconds
    DO_COLOR_WEEK_AYURVEDA, // Different color for each weekday according to the Ayurveda
    DO_COLOR_WEEK_THAI,     // Different color for each weekday according to the Thai
};

// Forward class definition
class DisplayStateManager;

///////////////////////////////////////////////////////////////////////////////
// Base class for all display states
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class DisplayStateBase : public ConsoleCallbacks
{
public:
    DisplayStateBase(std::string name)
        : m_name(name)
        , m_timeStamp(0)
        , m_pLEDs(NULL)
        , m_pConfig(NULL)
    {
    }

    virtual void Initialize(CRGB *pLEDs, BLEConfig *pConfig, DisplayStateManager *pManager) 
    {        
        m_pLEDs = pLEDs;
        m_pConfig = pConfig;
        m_timeStamp = millis();
        log("Initialize");
    };    

    virtual bool HandleLoop(unsigned long epochTime, time_t localTime) { return false; };        

    std::string getName() { return m_name; };

    static void setLayout(const ledclocklayout_t *layout);

protected:
    virtual CRGB ColorHandler(CRGB defaultColor, int brightness, int customParam = 0);

    void AddWordToLeds(const ledpos_t* pCurrentWord, CRGB defaultColor, int brightness, int customParam = 0);
    void FillBackground(void);
    int16_t CalcLedPos(int8_t x, int8_t y);
    uint32_t Elapsed(uint32_t ts);

    void  log(const char* format, ...);


protected:
    std::string                     m_name;
    uint32_t                        m_timeStamp;
    CRGB*                           m_pLEDs;
    BLEConfig*                      m_pConfig;
    static ledclocklayout_t         s_layout;
};
