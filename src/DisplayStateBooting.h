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

    virtual void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    virtual bool HandleLoop(unsigned long epochTime);  

private:
    int             m_nCounter;
    int             m_nDir;
    int             m_nCycleCounter;
};
