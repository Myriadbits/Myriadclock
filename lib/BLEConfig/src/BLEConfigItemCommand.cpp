///////////////////////////////////////////////////////////////////////////////
// MyriadclockSettings class
// Class for storing app wide settings
///////////////////////////////////////////////////////////////////////////////

#include "../include/BLEConfig.h"
#include "../include/BLEConfigItemCommand.h"
#include <stdio.h> 
#include <algorithm>
#include <time.h> // time_t, struct tm, time, mktime

//
// Store this config item to the preferences
void BLEConfigItemCommand::onStore(Preferences &preferences, char* pkey)
{    
    // Do not store the command in the preferences, just keep it in mem
    m_lastCommandID = m_value;
}
