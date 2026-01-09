#pragma once

#define FASTLED_INTERNAL
#include "FastLED.h"            // Fastled library to control the LEDs
#include "Console.h"
#include "ClockLayoutBase.h"
#include <Timezone.h>
#include "../lib/BLEConfig/include/BLEConfig.h"
#include "FontsBase.h"

#define MAXCOMMANDLEN               16
#define MAXCOMMANDDESCRIPTIONLEN    80

#define MAX_BIRTHDAYS       10
#define MAX_HOLIDAYS        10

//
// All configuration items
#define CONFIG_WIFI                     1
#define CONFIG_LOCATION                 2
#define CONFIG_LAYOUT                   3
#define CONFIG_TIMEZONE                 5
#define CONFIG_DAYLIGHTSAVING           6

#define CONFIG_COLOR_TIME               10
#define CONFIG_COLOR_WEEKDAY            11
#define CONFIG_COLOR_DATE               12
#define CONFIG_COLOR_BACKGROUND         13
#define CONFIG_BRIGHTNESS_DAY           14
#define CONFIG_BRIGHTNESS_NIGHT         15
#define CONFIG_BRIGHTNESS_BACKGROUND    16

#define CONFIG_OPTIONS_TIME             20
#define CONFIG_OPTIONS_WEEKDAY          21
#define CONFIG_OPTIONS_DATE             22
#define CONFIG_OPTIONS_CLOXEL           23

#define CONFIG_NAME                     30

#define CONFIG_COMMAND                  42



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

enum EUserCommands
{
    UC_NORMAL,              // Show normal clock
    UC_MATRIX,              // Show the matrix (fixed time)
    UC_ALLWORDS,            // Show all words (fixed time)
    UC_ANALOG,              // Show analog clock
};

enum EFontType
{
    FT_UNKNOWN,
    FT_56,
    FT_HIGHFONT48, // High font
    FT_HIGHFONT46, // High font small
    FT_582
};

enum ETextAlign
{
    TA_UNKNOWN = 0x00,
    TA_HCENTER = 0x01,
    TA_VCENTER = 0x02,
    TA_MIDTEXT = 0x04,
    TA_END = 0x08, // Align to the right/end side
};

inline ETextAlign operator|(ETextAlign a, ETextAlign b)
{
    return static_cast<ETextAlign>(static_cast<int>(a) | static_cast<int>(b));
}

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
        , m_pManager(NULL)
    {
    }

    virtual void Initialize(CRGB *pLEDs, BLEConfig *pConfig, DisplayStateManager *pManager) 
    {        
        m_pLEDs = pLEDs;
        m_pConfig = pConfig;
        m_timeStamp = millis();
        m_pManager = pManager;
        log("Initialize");
    };    

    virtual bool HandleLoop(unsigned long epochTime, time_t localTime) { return false; };        

    std::string getName() { return m_name; };

    static void setLayout(const ledclocklayout_t *layout);

protected:
    virtual CRGB ColorHandler(CRGB defaultColor, int brightness, int customParam = 0);

    void    AddWordToLeds(const ledpos_t* pCurrentWord, CRGB defaultColor, int brightness, int customParam = 0);    
    void    FillBackground(const int brightness);
    void    DrawGFX(CRGB *pLEDs, EFontType fontType, ETextAlign align, int x, int y, std::string text, CRGB color);
    int     CalculateGFXWidth(const GFXfont* font, std::string text);
    int16_t CalcLedPos(int8_t x, int8_t y);
    uint32_t Elapsed(uint32_t ts);

    void    log(const char* format, ...);

    void    CalcSunriseSunset(unsigned long timestamp, float lat, float lon, float alt, unsigned long *sunrise, unsigned long *sunset);
    int     GetBrightness(unsigned long epochTime);

protected:
    std::string                     m_name;
    uint32_t                        m_timeStamp;
    CRGB*                           m_pLEDs;
    BLEConfig*                      m_pConfig;
    static ledclocklayout_t         s_layout;
    DisplayStateManager*            m_pManager;
};
