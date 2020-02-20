#pragma once

#include "Esp.h"
#include "FastLED.h"
#include <Preferences.h>

///////////////////////////////////////////////////////////////////////////////
// Settings class
///////////////////////////////////////////////////////////////////////////////
class MyriadclockSettings
{
private:
    Preferences m_preferences;

public:
    uint32_t colTime;
    uint32_t colDate;    

    void Initialize();
    void Store();
    void Load();
};
