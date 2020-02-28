#pragma once

#include "Esp.h"
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
    enum EDisplayOptions
    {
        DO_NORMAL,              // Single color
        DO_COLOR_CYCLENORMAL,   // Normal color cycle
        DO_COLOR_CYCLEHOUR,     // Color cycle per hour
        DO_COLOR_CYCLEDAY,      // Every day another color
        DO_COLOR_PARTY_SLOW,    // Random per character per minute
        DO_COLOR_PARTY_QUICK,   // Random per character constant
        DO_COLOR_PARTY_MINUTE,  // Random colors every minute for a few seconds
        DO_COLOR_WEEK_AYURVEDA, // Different color for each weekday according to the Ayurveda
        DO_COLOR_WEEK_THAI,     // Different color for each weekday according to the Thai
    };

    uint32_t    colTime;   // Color of time part
    uint32_t    colWeekday;// Color of week day
    uint32_t    colDate;   // Color of date part
    uint32_t    colPulse;  // Color of the heartbeat pulse led

    int16_t     nBrightnessDay;     // Day brightness
    int16_t     nBrightnessNight;   // Day night

    EDisplayOptions  eDisplayOptionsTime;    
    EDisplayOptions  eDisplayOptionsWeekday;    
    EDisplayOptions  eDisplayOptionsDate;    
    EDisplayOptions  eDisplayOptionsBirthday;
    EDisplayOptions  eDisplayOptionsHoliday;

    time_t      dateBirthdays[MAX_BIRTHDAYS];   // Special days
    time_t      dateHolidays[MAX_HOLIDAYS];     // Holidays

    void Initialize();
    void Store();
    void Load();

};
