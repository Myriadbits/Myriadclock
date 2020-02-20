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
    m_preferences.putUInt("colorDate", colDate);
}

//
// Load all settings from non-volatile storage
//
void MyriadclockSettings::Load()
{
    colTime = m_preferences.getUInt("colorTime", 0x00FF00);
    colDate = m_preferences.getUInt("colorDate", 0xFFA500);
}



