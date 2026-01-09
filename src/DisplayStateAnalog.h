#pragma once

#include "DisplayStateBase.h"

///////////////////////////////////////////////////////////////////////////////
// Show analog clock :)
///////////////////////////////////////////////////////////////////////////////
class DisplayStateAnalog : public DisplayStateBase
{
public:
    DisplayStateAnalog() : DisplayStateBase("analog") 
    {
    }

    virtual void Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager);
    virtual bool HandleLoop(unsigned long epochTime, time_t localTime);  

private:
    uint8_t       m_nWordIndexTop;
    uint8_t       m_nWordIndexBottom;
    uint8_t       m_nWordIndexDay;
    uint8_t       m_nLoopCounter;
};
