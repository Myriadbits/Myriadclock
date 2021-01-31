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

    virtual void Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager);
    virtual bool HandleLoop(unsigned long epochTime, time_t localTime);  

private:
    int             m_nCounter;
    int             m_nDir;
};
