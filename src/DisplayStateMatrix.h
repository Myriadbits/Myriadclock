#pragma once

#include "DisplayStateBase.h"

#define MAX_COLS     32

///////////////////////////////////////////////////////////////////////////////
// Show the matrix animation
///////////////////////////////////////////////////////////////////////////////
class DisplayStateMatrix : public DisplayStateBase
{
public:
    DisplayStateMatrix() : DisplayStateBase("matrix") 
    {
    }

    virtual void Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager);
    virtual bool HandleLoop(unsigned long epochTime, time_t localTime);  

private:
    uint8_t     m_introPos[MAX_COLS];
    uint8_t     m_introLen[MAX_COLS];
    uint8_t     m_animationStep = 100; // 1 step = x ms
    uint32_t    m_maxTotalTime = 0;
    uint32_t    m_totalTime = 0;

    uint8_t       m_nWordIndexTop;
    uint8_t       m_nWordIndexBottom;
    uint8_t       m_nWordIndexDay;
};
