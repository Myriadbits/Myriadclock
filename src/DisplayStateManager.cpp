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

#include "ClockLayoutNL_V1.h"
#include "ClockLayoutNL_V2.h"
#include "ClockLayoutEN_V1.h"

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

    // Start the default state
    changeState(m_eDefaultState);

    // Load the last layout
    setLayout(pSettings->nLayout);
}

//
// Change/set the state
//
void DisplayStateManager::changeState(const EDisplayState eState)
{
    m_eNewState = eState;
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
    else if (command == "layout" && args.size() > 0)
    {
        int value = atoi(args[1].c_str());
        m_pSettings->nLayout = value;
        m_pSettings->Store();

        setLayout(value);
    }
}


//
// Set the clock layout
void DisplayStateManager::setLayout(const int nIndex) 
{   
    Serial.printf("Layout: %d\n", nIndex);

    // Default is new dutch layout
    ledclocklayout_t *playout = const_cast<ledclocklayout_t*>(&s_layoutNL_V2);

    // Get the correct layout
    if (nIndex == 1) playout = const_cast<ledclocklayout_t*>(&s_layoutNL_V1);
    if (nIndex == 2) playout = const_cast<ledclocklayout_t*>(&s_layoutEN_V1);
    
    // Inform all display states of the new layout
    DisplayStateBase::setLayout(playout);    
};

//
// Pass control to through the current display state
//
void DisplayStateManager::handleLoop(unsigned long epochTime)
{
    if (m_eNewState != m_eCurrentState)
    {
        auto it = m_states.find(m_eNewState);
        if (it != m_states.end())
        {
            it->second->Initialize(m_pLEDs, m_pTZ, m_pSettings);
            m_eCurrentState = m_eNewState;
        }        
    }

    auto it = m_states.find(m_eCurrentState);
    if (it != m_states.end())
    {
        if (!it->second->HandleLoop(epochTime))
        {
            // When handleloop returns false, fallback to the default state
            changeState(m_eDefaultState);
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
    auto it = m_states.find(m_eNewState);
    if (it != m_states.end())
    {
        DisplayStatePasscode* passcodeState = (DisplayStatePasscode*)it->second;
        if (passcodeState)
            passcodeState->setResult(success);
    }
}

