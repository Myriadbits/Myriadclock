#pragma once

#include "FastLED.h"            // Fastled library to control the LEDs
#include "ClockLayout.h"
#include <Timezone.h>
#include "MyriadclockSettings.h"

#define MAXCOMMANDLEN               16
#define MAXCOMMANDDESCRIPTIONLEN    80

///////////////////////////////////////////////////////////////////////////////
// Base class for all display states
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class DisplayStateBase
{
public:
    virtual void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings) 
    {
        m_pLEDs = pLEDs;
        m_pTZ = pTZ;
        m_pSettings = pSettings;
        m_timeStamp = millis();
        memset(m_sCommand, 0, MAXCOMMANDLEN); 
    };
    
    virtual bool HandleLoop(unsigned long epochTime) { return false; };        

    char* GetCommand() { return m_sCommand; }
    char* GetCommandDescription() { return m_sCommandDescription; }

    virtual void CommandHandler(int, char **) {};

protected:
    virtual CRGB ColorHandler(uint32_t customParam);

    void AddWordToLeds(ledpos_t* pCurrentWord, uint32_t customParam);
    int16_t CalcLedPos(int8_t x, int8_t y);
    uint32_t Elapsed(uint32_t ts);

protected:
    char         m_sCommand[MAXCOMMANDLEN];
    char         m_sCommandDescription[MAXCOMMANDDESCRIPTIONLEN];
    uint32_t     m_timeStamp;
    CRGB*        m_pLEDs;
    Timezone*    m_pTZ;
    MyriadclockSettings*    m_pSettings;
};
