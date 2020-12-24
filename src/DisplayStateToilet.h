#pragma once

#include "DisplayStateBase.h"

///////////////////////////////////////////////////////////////////////////////
// Base class for all display states
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class DisplayStateToilet : public DisplayStateBase
{
public:
    DisplayStateToilet() : DisplayStateBase("toilet") 
    {
    }

    void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    bool HandleLoop(unsigned long epochTime);  

private:
    static uint8_t     s_toiletSign[16 * 16];
    uint8_t     m_nWordIndexTop;
    uint8_t     m_nWordIndexBottom;
};
