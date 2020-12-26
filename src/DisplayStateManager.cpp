///////////////////////////////////////////////////////////////////////////////
// StateManager class
// Class that manages all display states
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateManager.h"

#include "DisplayStateNoWiFi.h"
#include "DisplayStateBooting.h"
#include "DisplayStateWords.h"
#include "DisplayStateClock.h"
#include "DisplayStateUpdating.h"
#include "DisplayStatePasscode.h"
#include "DisplayStateToilet.h"

//
//  Initialize/setup this class
//
void DisplayStateManager::initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings)
{ 
    m_pLEDs = pLEDs;
    m_pTZ = pTZ;
    m_pSettings = pSettings;

    // Add all states to our statemachine
    addState(DS_NOWIFI, new DisplayStateNoWiFi());
    addState(DS_BOOTING, new DisplayStateBooting());
    addState(DS_CLOCK,  new DisplayStateClock());
    addState(DS_WORDS, new DisplayStateWords());
    addState(DS_UPDATING, new DisplayStateUpdating());
    addState(DS_PASSCODE, new DisplayStatePasscode());
    addState(DS_TOILET, new DisplayStateToilet());

    Console::getInstance().add("state", this, "Switch the display state");
    Console::getInstance().add("layout", this, "Switch the layout");
}

//
// Change/set the state
//
void DisplayStateManager::changeState(const EDisplayState eState)
{
    auto it = m_states.find(eState);
    if (it != m_states.end())
    {
        m_pCurrentState = it->second;
        m_pCurrentState->Initialize(m_pLEDs, m_pTZ, m_pSettings);
        
    }
}

//
// Add a new state to the state array
//
void DisplayStateManager::addState(EDisplayState eState, DisplayStateBase* pNewState)
{
    pNewState->Initialize(m_pLEDs, m_pTZ, m_pSettings);
    m_states.insert(std::pair<EDisplayState, DisplayStateBase*>(eState, pNewState));
}

//
// A command is called
//
void DisplayStateManager::commandHandler(std::string command, std::vector<std::string> args)
{
    if (command == "state" && args.size() > 0)
    {
        // Find the name of the command in the state array
        for (auto const& it : m_states)
        {
            if (args[1] == it.second->getName())
            {
                // Found it, change the state
                changeState(it.first);
                break;
            }
        }
    }
    if (command == "layout" && args.size() > 0)
    {
        if (args[1] == "0") DisplayStateBase::setLayout(const_cast<ledclocklayout_t*>(&s_layoutNL_V1));
        if (args[1] == "1") DisplayStateBase::setLayout(const_cast<ledclocklayout_t*>(&s_layoutNL_V2));
    }
}

//
// Pass control to through the current display state
//
void DisplayStateManager::handleLoop(unsigned long epochTime)
{
    if (m_pCurrentState != NULL)
    {
        if (!m_pCurrentState->HandleLoop(epochTime))
        {
            // When handleloop returns false, select the next handler
            changeState(DS_CLOCK);
        }
    }
}

//
// A BT connection request arrives, display the passcode
// 
void DisplayStateManager::onDisplayPassKey(uint32_t passkey)
{
    if (m_pSettings)
        m_pSettings->bluetoothPasscode = passkey;
    changeState(DS_PASSCODE);
}

//
// A BT connection has been established or failed
// 
void DisplayStateManager::onBluetoothConnection(bool success)
{
    DisplayStatePasscode* passcodeState = (DisplayStatePasscode*)m_pCurrentState;
    if (passcodeState)
        passcodeState->setResult(success);
}

