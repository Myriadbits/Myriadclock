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

    virtual void Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager);
    virtual bool HandleLoop(unsigned long epochTime, time_t localTime);  

private:
    int             m_nCounter;
    int             m_nDir;
    int             m_nCycleCounter;
};
