///////////////////////////////////////////////////////////////////////////////
// MyriadclockSettings class
// Class for storing app wide settings
///////////////////////////////////////////////////////////////////////////////

#include "MyriadclockSettings.h"

//
// Initialize, read all settings
//
void MyriadclockSettings::Initialize()
{
    m_preferences.begin("Myriadclock", false);
    Load();
}

//
// Store all settings to non-volatile storage
//
void MyriadclockSettings::Store()
{
    m_preferences.putUInt("colorTime", colTime);
    m_preferences.putUInt("colorWeekday", colWeekday);
    m_preferences.putUInt("colorDate", colDate);
    m_preferences.putShort("brightnessDay", nBrightnessDay);
    m_preferences.putShort("brightnessNight", nBrightnessNight);
    m_preferences.putInt("doTime", eDisplayOptionsTime);
    m_preferences.putInt("doWeekday", eDisplayOptionsWeekday);
    m_preferences.putInt("doDate", eDisplayOptionsDate);
    m_preferences.putInt("doBirthdays", eDisplayOptionsBirthday);
    m_preferences.putInt("doHolidays", eDisplayOptionsHoliday);

    char buff[32];
    for(int n = 0; n < MAX_BIRTHDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "birthdays[%d]", n);
        m_preferences.putULong64(buff, dateBirthdays[n]);
    }
    for(int n = 0; n < MAX_HOLIDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "holidays[%d]", n);
        m_preferences.putULong64(buff, dateHolidays[n]);
    }
}

//
// Load all settings from non-volatile storage
//
void MyriadclockSettings::Load()
{
    colTime = m_preferences.getUInt("colorTime", 0x00FF00);
    colWeekday = m_preferences.getUInt("colorWeekday", 0xFFA500);
    colDate = m_preferences.getUInt("colorDate", 0xE59400);

    nBrightnessDay = m_preferences.getShort("brightnessDay", 100);
    nBrightnessNight = m_preferences.getShort("brightnessNight", 30);
    eDisplayOptionsTime = (EDisplayOptions) m_preferences.getInt("doTime", DO_NORMAL);
    eDisplayOptionsWeekday = (EDisplayOptions) m_preferences.getInt("doWeekday", DO_NORMAL);
    eDisplayOptionsDate = (EDisplayOptions) m_preferences.getInt("doDate", DO_NORMAL);
    eDisplayOptionsBirthday = (EDisplayOptions) m_preferences.getInt("doBirthdays", DO_COLOR_PARTY_MINUTE);
    eDisplayOptionsHoliday = (EDisplayOptions) m_preferences.getInt("doHolidays", DO_NORMAL);

    char buff[32];
    for(int n = 0; n < MAX_BIRTHDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "birthdays[%d]", n);
        if (n == 0) dateBirthdays[n] = m_preferences.getULong64(buff, 62352000L); // 23-12-1971
        else dateBirthdays[n] = m_preferences.getULong64(buff, 0L);
    }
    for(int n = 0; n < MAX_HOLIDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "holidays[%d]", n);
        if (n == 0) dateHolidays[n] = m_preferences.getULong64(buff, 946728000L); // New years day
        else if (n == 1) dateHolidays[n] = m_preferences.getULong64(buff, 977745600L); // Christmas 1
        else if (n == 2) dateHolidays[n] = m_preferences.getULong64(buff, 977832000L); // Christmas 2
        else dateHolidays[n] = m_preferences.getULong64(buff, 0L);
    }

}



