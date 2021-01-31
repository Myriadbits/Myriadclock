#pragma once

#include "DisplayStateBase.h"

///////////////////////////////////////////////////////////////////////////////
// Base class for all display states
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class DisplayStateWords : public DisplayStateBase
{
public:
    DisplayStateWords() : DisplayStateBase("words") 
    {
    }

    virtual void Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager);
    virtual bool HandleLoop(unsigned long epochTime, time_t localTime);  

private:
    uint8_t       m_nWordIndexTop;
    uint8_t       m_nWordIndexBottom;
    uint8_t       m_nWordIndexDay;
};
