#pragma once

#include "FastLED.h"            // Fastled library to control the LEDs
#include "Console.h"
#include "ClockLayoutNL_V1.h"
#include "ClockLayoutNL_V2.h"
#include <Timezone.h>
#include "MyriadclockSettings.h"

#define MAXCOMMANDLEN               16
#define MAXCOMMANDDESCRIPTIONLEN    80

///////////////////////////////////////////////////////////////////////////////
// Base class for all display states
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class DisplayStateBase : public ConsoleHandler
{
public:
    DisplayStateBase(std::string name)
        : m_name(name)
    {
    }

    virtual void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings) 
    {        
        setLayout(&s_layoutNL_V2);
        m_pLEDs = pLEDs;
        m_pTZ = pTZ;
        m_pSettings = pSettings;
        m_timeStamp = millis();
    };    

    virtual bool HandleLoop(unsigned long epochTime) { return false; };        

    std::string getName() { return m_name; };

    static void setLayout(const ledclocklayout_t *layout);

protected:
    virtual CRGB ColorHandler(CRGB defaultColor, int customParam = 0);

    void AddWordToLeds(const ledpos_t* pCurrentWord, CRGB defaultColor, int customParam = 0);
    void FillBackground(void);
    int16_t CalcLedPos(int8_t x, int8_t y);
    uint32_t Elapsed(uint32_t ts);

protected:
    std::string                     m_name;
    uint32_t                        m_timeStamp;
    CRGB*                           m_pLEDs;
    Timezone*                       m_pTZ;
    MyriadclockSettings*            m_pSettings;
    static ledclocklayout_t         s_layout;
};
