#pragma once

#include <map>

#include "FastLED.h"            // Fastled library to control the LEDs
#include "Console.h"
#include "ClockLayoutNL_V1.h"
#include "ClockLayoutNL_V2.h"
#include <Timezone.h>
#include "MyriadclockSettings.h"
#include "DisplayStateBase.h"
#include "MIOTConfigurator.h"

#define MAXCOMMANDLEN               16
#define MAXCOMMANDDESCRIPTIONLEN    80
#define MAXSTATES                   10      // Maximum number of LED states

//
// All possible display states
//
enum EDisplayState
{
    DS_NOWIFI,
    DS_BOOTING,
    DS_CLOCK,
    DS_WORDS,
    DS_UPDATING,
    DS_PASSCODE,
    DS_TOILET
};


///////////////////////////////////////////////////////////////////////////////
// Base class for all display states
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class DisplayStateManager : public ConsoleCallbacks, public MIOTCallbacks
{
public:
    DisplayStateManager()
        : m_pCurrentState(NULL)
    {    
    }

    void    changeState(const EDisplayState estate);

    void    initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    void    addState(EDisplayState eState, DisplayStateBase* pNewState);
    void    handleLoop(unsigned long epochTime);

    // Console Callbacks
    virtual void commandHandler(std::string command, std::vector<std::string> args);

    // MIOT Callbacks
    virtual void onDisplayPassKey(uint32_t passkey);
    virtual void onBluetoothConnection(bool success);

protected:
    DisplayStateBase*                           m_pCurrentState;
    std::map<EDisplayState, DisplayStateBase*>  m_states;

    CRGB*                           m_pLEDs;
    Timezone*                       m_pTZ;
    MyriadclockSettings*            m_pSettings;
    static const ledclocklayout_t*  s_pLayout;
};
