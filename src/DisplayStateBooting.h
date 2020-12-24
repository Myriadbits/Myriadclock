#pragma once

#include "DisplayStateBase.h"

///////////////////////////////////////////////////////////////////////////////
// Booting display state
///////////////////////////////////////////////////////////////////////////////
class DisplayStateBooting : public DisplayStateBase
{
public:
    DisplayStateBooting() : DisplayStateBase("booting") 
    {
    }

    void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    bool HandleLoop(unsigned long epochTime);  

private:
    int             m_nCounter;
    int             m_nDir;
    int             m_nCycleCounter;
};
