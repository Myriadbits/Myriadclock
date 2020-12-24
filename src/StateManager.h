#pragma once

#include <vector>

#include "FastLED.h"            // Fastled library to control the LEDs
#include "Console.h"
#include "ClockLayoutNL_V1.h"
#include "ClockLayoutNL_V2.h"
#include <Timezone.h>
#include "MyriadclockSettings.h"
#include "DisplayStateBase.h"

#define MAXCOMMANDLEN               16
#define MAXCOMMANDDESCRIPTIONLEN    80
#define MAXSTATES                   10      // Maximum number of LED states


///////////////////////////////////////////////////////////////////////////////
// Base class for all display states
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class StateManager : public ConsoleHandler
{
public:
    StateManager()
        : m_nCurrentState(0)
    {    
    }

    void    setState(const int number);

    void    initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    void    addState(DisplayStateBase* pNewState);
    virtual void commandHandler(std::string command, std::vector<std::string> args);
    virtual bool handleLoop(unsigned long epochTime);

protected:
    int                             m_nCurrentState;
    std::vector<DisplayStateBase*>  m_states;

    CRGB*                           m_pLEDs;
    Timezone*                       m_pTZ;
    MyriadclockSettings*            m_pSettings;
    static const ledclocklayout_t*  s_pLayout;
};
