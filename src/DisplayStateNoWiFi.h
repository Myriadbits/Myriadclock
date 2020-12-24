#pragma once

#include "DisplayStateBase.h"

///////////////////////////////////////////////////////////////////////////////
// NoWiFi display state
///////////////////////////////////////////////////////////////////////////////
class DisplayStateNoWiFi : public DisplayStateBase
{
public:
    DisplayStateNoWiFi() : DisplayStateBase("nowifi") 
    {
    }

    void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    bool HandleLoop(unsigned long epochTime);  

private:
    int             m_nCounter;
    int             m_nDir;
    int             m_nCycleCounter;
};
