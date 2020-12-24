///////////////////////////////////////////////////////////////////////////////
// StateManager class
// Class that manages all display states
///////////////////////////////////////////////////////////////////////////////

#include "StateManager.h"

#include "DisplayStateNoWiFi.h"
#include "DisplayStateBooting.h"
#include "DisplayStateWords.h"
#include "DisplayStateClock.h"
#include "DisplayStateUpdating.h"
#include "DisplayStateToilet.h"

//
//  Initialize/setup this class
//
void StateManager::initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings)
{ 
    m_pLEDs = pLEDs;
    m_pTZ = pTZ;
    m_pSettings = pSettings;

    // Add all states to our statemachine
    addState(new DisplayStateNoWiFi());
    addState(new DisplayStateBooting());
    addState(new DisplayStateClock());
    addState(new DisplayStateWords());
    addState(new DisplayStateUpdating());
    addState(new DisplayStateToilet());

    Console::getInstance().add("state", this, "Switch the display state");
    Console::getInstance().add("layout", this, "Switch the layout");
}

//
// Change/set the state
//
void StateManager::setState(const int number)
{
    if (number < m_states.size())
    {
        m_nCurrentState = number;        
    }
}

//
// Add a new state to the state array
//
void StateManager::addState(DisplayStateBase* pNewState)
{
    pNewState->Initialize(m_pLEDs, m_pTZ, m_pSettings);
    m_states.push_back(pNewState);
}

//
// A command is called
//
void StateManager::commandHandler(std::string command, std::vector<std::string> args)
{
    if (command == "state" && args.size() > 0)
    {
        // Find the command in the state array
        for(int n = 0; n < m_states.size(); n++)
        {
            if (args[1] == m_states[n]->getName())
            {
                m_nCurrentState = n;
                m_states[n]->Initialize(m_pLEDs, m_pTZ, m_pSettings); // Reinitialize 
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
bool StateManager::handleLoop(unsigned long epochTime)
{
    if (!m_states[m_nCurrentState]->HandleLoop(epochTime))
    {
        // When handleloop returns false, select the next handler
        m_nCurrentState = 0; // TODO always fallback to clock?
        return false;
    }
    return true;
}


