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

    virtual void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    virtual bool HandleLoop(unsigned long epochTime);  

private:
    uint8_t       m_nWordIndexTop;
    uint8_t       m_nWordIndexBottom;
    uint8_t       m_nWordIndexDay;
};
