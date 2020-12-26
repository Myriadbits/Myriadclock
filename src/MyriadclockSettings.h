#pragma once

#include "FastLED.h"
#include <Preferences.h>

#define MAX_BIRTHDAYS       10
#define MAX_HOLIDAYS        10

///////////////////////////////////////////////////////////////////////////////
// Settings class
///////////////////////////////////////////////////////////////////////////////
class MyriadclockSettings
{
private:
    Preferences m_preferences;

public:
    MyriadclockSettings()
        : sClockName()
        , nSerialNumber(0)
        , nBrightnessDay(80)
        , nBrightnessNight(30)
        , nBrightnessBackground(4)
        , eDisplayOptionsTime(DO_NORMAL)    
        , eDisplayOptionsWeekday(DO_NORMAL) 
        , eDisplayOptionsDate(DO_NORMAL)
        , eDisplayOptionsBirthday(DO_COLOR_PARTY_MINUTE)
        , eDisplayOptionsHoliday(DO_COLOR_PARTY_MINUTE)
        , bluetoothPasscode(0)
    {
        
    }

    enum EDisplayOptions
    {
        DO_NORMAL,              // Single color
        DO_COLOR_CYCLENORMAL,   // Normal color cycle
        DO_COLOR_CYCLEHOUR,     // Color cycle per hour
        DO_COLOR_CYCLERESERVED, // <reserved>
        DO_COLOR_PARTY_SLOW,    // Random per character per minute
        DO_COLOR_PARTY_QUICK,   // Random per character constant
        DO_COLOR_PARTY_MINUTE,  // Random colors every minute for a few seconds
        DO_COLOR_WEEK_AYURVEDA, // Different color for each weekday according to the Ayurveda
        DO_COLOR_WEEK_THAI,     // Different color for each weekday according to the Thai
    };

    String      sClockName;     // Name of the clock, filled in by the main setup (not stored to DB)
    uint16_t    nSerialNumber;  // Serial number (not stored to db)

    uint32_t    colTime;   // Color of time part
    uint32_t    colWeekday;// Color of week day
    uint32_t    colDate;   // Color of date part
    uint32_t    colPulse;  // Color of the heartbeat pulse led
    uint32_t    colBackground;  // Color of the heartbeat pulse led

    int16_t     nBrightnessDay;     // Day brightness
    int16_t     nBrightnessNight;   // Day night
    int16_t     nBrightnessBackground;  // Background brightness

    EDisplayOptions  eDisplayOptionsTime;    
    EDisplayOptions  eDisplayOptionsWeekday;    
    EDisplayOptions  eDisplayOptionsDate;    
    EDisplayOptions  eDisplayOptionsBirthday;
    EDisplayOptions  eDisplayOptionsHoliday;

    time_t      dateBirthdays[MAX_BIRTHDAYS];   // Special days
    time_t      dateHolidays[MAX_HOLIDAYS];     // Holidays

    // Volatile parameters
    uint32_t    bluetoothPasscode; 

    void Initialize();
    void Store();
    void Load();

};
