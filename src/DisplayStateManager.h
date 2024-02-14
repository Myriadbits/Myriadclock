#pragma once

#include <map>

#define FASTLED_INTERNAL
#include "FastLED.h"            // Fastled library to control the LEDs
#include "Console.h"

#include <Timezone.h>
#include "../lib/BLEConfig/include/BLEConfig.h"
#include "DisplayStateBase.h"

#define MAXCOMMANDLEN               16
#define MAXCOMMANDDESCRIPTIONLEN    80
#define MAXSTATES                   10      // Maximum number of LED states


//
// All possible display states
//
enum EDisplayState
{
    DS_NONE,
    DS_NOWIFI,
    DS_BOOTING,
    DS_CLOCK,
    DS_WORDS,
    DS_UPDATING,
    DS_PASSCODE,
    DS_TOILET,
    DS_MATRIX, // New as of 1.0.2?
    DS_ANALOG,
};


///////////////////////////////////////////////////////////////////////////////
// Base class for all display states
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class DisplayStateManager : public ConsoleCallbacks, public IBLEConfigCallbacks
{
public:
    DisplayStateManager()
        : m_eCurrentState(DS_NONE)
        , m_eDefaultState(DS_BOOTING)
        , m_bluetoothPasscode(0)
    {    
    }

    void    setDefaultState(const EDisplayState estate) { m_eDefaultState = estate; };
    void    changeState(const EDisplayState estate);

    void    initialize(CRGB* pLEDs, BLEConfig* config);
    void    handleLoop(unsigned long epochTime);

    uint32_t getBluetoothPasscode() { return m_bluetoothPasscode; }

    // Console Callbacks
    virtual void commandHandler(std::string command, std::vector<std::string> args);

    // MIOT Callbacks
    virtual void onDisplayPassKey(uint32_t passkey);
    virtual void onBluetoothConnection(bool success);
    virtual void onConfigItemChanged(BLEConfigItemBase *pconfigItem);

private:
    void    addState(EDisplayState eState, DisplayStateBase* pNewState);
    void    setLayout(const int nIndex);
    void    setTimezone();

protected:
    EDisplayState                               m_eCurrentState;
    EDisplayState                               m_eNewState;
    EDisplayState                               m_eDefaultState;
    std::map<EDisplayState, DisplayStateBase*>  m_states;

    // Volatile parameters
    uint32_t                        m_bluetoothPasscode; 

    CRGB*                           m_pLEDs;
    Timezone*                       m_pTZ;
    BLEConfig*                      m_pConfig;
    uint32_t                        m_timezoneChangedCountdown;
    static const ledclocklayout_t*  s_pLayout;
};
