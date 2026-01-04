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
#include "DisplayStateMatrix.h"

#include "ClockLayoutNL_V1.h"
#include "ClockLayoutNL_V2.h"
#include "ClockLayoutEN_V1.h"
#include "ClockLayoutEN_V2.h"

#define STARTUP_TIME                10000 // X milliseconds startup time (before switching to no-wifi)
#define TIMEZONE_CHANGED_TIME        5000 // X milliseconds time after switching timezones

// Australia 
TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    // UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    // UTC + 10 hours

// US 


// UTC
TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};     // UTC


// Fake reset function
void(* resetFunc) (void) = 0;//declare reset function at address 0

//
//  Initialize/setup this class
//
void DisplayStateManager::initialize(CRGB* pLEDs, BLEConfig* pConfig)
{ 
    m_pLEDs = pLEDs;
    m_pTZ = NULL;
    m_pConfig = pConfig;
    m_timezoneChangedCountdown = 0;

    // Add all states to our statemachine
    addState(DS_NOWIFI, new DisplayStateNoWiFi());
    addState(DS_BOOTING, new DisplayStateBooting());
    addState(DS_CLOCK,  new DisplayStateClock());
    addState(DS_WORDS, new DisplayStateWords());
    addState(DS_UPDATING, new DisplayStateUpdating());
    addState(DS_PASSCODE, new DisplayStatePasscode());
    addState(DS_MATRIX, new DisplayStateMatrix());

    Console::getInstance().add("state", this, "Switch the display state");
    Console::getInstance().add("layout", this, "Switch the layout");
    Console::getInstance().add("reset", this, "Reset");

    // Start the default state
    changeState(m_eDefaultState);

    // Load the last layout
    setLayout(m_pConfig->getConfigValue(CONFIG_LAYOUT));

    // And initialize the timezone
    setTimezone();
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
    pNewState->Initialize(m_pLEDs, m_pConfig, this);
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

        if (m_pConfig != NULL)
        {
            BLEConfigItemUInt32* pItem = (BLEConfigItemUInt32*) m_pConfig->getConfigItem(CONFIG_LAYOUT);
            if (pItem != NULL)
                pItem->setValue(value); // Change the layout value
            m_pConfig->store();
        }        
        setLayout(value);
    }
    else if (command == "reset")
    {
        resetFunc();
    }
}


//
// Set the clock layout
void DisplayStateManager::setLayout(const int nIndex) 
{   
    // Default is new dutch layout
    ledclocklayout_t *playout = const_cast<ledclocklayout_t*>(&s_layoutNL_V2);

    // Get the correct layout
    if (nIndex == 1) playout = const_cast<ledclocklayout_t*>(&s_layoutNL_V1);
    if (nIndex == 2) playout = const_cast<ledclocklayout_t*>(&s_layoutEN_V1);
    if (nIndex == 3) playout = const_cast<ledclocklayout_t*>(&s_layoutEN_V2);
    
    // Inform all display states of the new layout
    DisplayStateBase::setLayout(playout);    
}


//
// Set the clock layout
void DisplayStateManager::setTimezone() 
{   
    // Get the timezone
    m_timezoneChangedCountdown = millis() + TIMEZONE_CHANGED_TIME;
    int timezone = m_pConfig->getConfigValue(CONFIG_TIMEZONE);
    timezone -= 12;

    // Calculate Daylight saving time
    delete m_pTZ;
    m_pTZ = NULL;
    switch (m_pConfig->getConfigValue(CONFIG_DAYLIGHTSAVING))
    {
        default:
        case 0: // Off
            {
                TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, (timezone * 60)};     // UTC
                m_pTZ = new Timezone(utcRule);
            }
            break;
        case 1: // Central European
            {
                TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, (timezone * 60) + 60}; // Central European Summer Time
                TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, (timezone * 60)}; // Central European Standard Time
                m_pTZ = new Timezone(CEST, CET);
            }
            break;
        case 2: // United Kingdom
            {
                TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, (timezone * 60) + 60}; // British Summer Time
                TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, (timezone * 60)}; // Standard Time
                m_pTZ = new Timezone(BST, GMT);
            }
            break;
        case 3: // Australia
            {
                TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, (timezone * 60)}; // UTC + 11 hours
                TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, (timezone * 60) - 60}; // UTC + 10 hours
                m_pTZ = new Timezone(aEDT, aEST);
            }
            break;
        case 4: // US
            {
                TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, (timezone * 60) + 60}; // Eastern Daylight Time = UTC - 4 hours (-240)
                TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, (timezone * 60)}; // Eastern Standard Time = UTC - 5 hours (-300)
                m_pTZ = new Timezone(usEDT, usEST);
            }
            break;
    }
}


//
// Pass control to through the current display state
//
void DisplayStateManager::handleLoop(unsigned long epochTime)
{
    // Convert to local time
    TimeChangeRule *tcr;    
    time_t tLocal = (m_pTZ != NULL) ? m_pTZ->toLocal(epochTime, &tcr) : 0; // (Note: tcr cannot be NULL)

    if (m_eNewState != m_eCurrentState)
    {
        auto it = m_states.find(m_eNewState);
        if (it != m_states.end())
        {
            it->second->Initialize(m_pLEDs, m_pConfig, this);
            m_eCurrentState = m_eNewState;
        }        
    }
    
    // If there is a WiFi config item: update the WIFi config item
    if (m_pConfig != NULL)
    {
        BLEConfigItemWiFi* pciWiFi = reinterpret_cast<BLEConfigItemWiFi*>(m_pConfig->getConfigItem(CONFIG_WIFI));
        if (pciWiFi != NULL)
        {
            pciWiFi->setConnected(WiFi.status() == WL_CONNECTED);
        }
    }

    auto it = m_states.find(m_eCurrentState);
    if (it != m_states.end())
    {
        if (!it->second->HandleLoop(epochTime, tLocal))
        {
            // When handleloop returns false, fallback to the default state
            changeState(m_eDefaultState);
        }
    }

    // When there is no wifi show the NoWiFi (when we have been up and running for a small timespan)
    if ((millis() > STARTUP_TIME) && (m_eCurrentState != DS_NOWIFI) && (WiFi.status() != WL_CONNECTED))
    {
        // Fallback to the no-wifi state
        changeState(DS_NOWIFI);
        BLECONFIG_LOG("Switching to NOWIFI");
    }
    else if ((m_eCurrentState == DS_NOWIFI) && (WiFi.status() == WL_CONNECTED))
    {
        // When we were showing the no-wifi and the WiFi is connected, jump back to booting
        changeState(DS_BOOTING);
        BLECONFIG_LOG("Switching to BOOTING");
    }

    // Some specials:
    int nHours = hour(tLocal);
    int nMinutes = minute(tLocal);
    if (nHours == 0 && nMinutes == 0 && millis() > m_timezoneChangedCountdown) 
    {
        // Show matrix (for max 1 minute)
        changeState(DS_MATRIX);
    }
}

//
// A BT connection request arrives, display the passcode
// 
void DisplayStateManager::onDisplayPassKey(uint32_t passkey)
{
    m_bluetoothPasscode = passkey;
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

//
// A config item has changed, forward to the settings
// 
void DisplayStateManager::onConfigItemChanged(BLEConfigItemBase *pconfigItem)
{
    if (pconfigItem != NULL)
    {
        switch (pconfigItem->getId())
        {
            case CONFIG_LAYOUT:
                {
                    // Clock layout has changed, 
                    BLEConfigItemOption* pconfig = (BLEConfigItemOption*) pconfigItem;
                    setLayout(pconfig->getValue());
                }
                break;

            case CONFIG_TIMEZONE:
            case CONFIG_DAYLIGHTSAVING:
                setTimezone();
                break;

            case CONFIG_COMMAND:
                {
                    BLEConfigItemCommand* pconfig = (BLEConfigItemCommand*) pconfigItem;
                    BLECONFIG_LOG("Executing User Command: %d", pconfig->getLastCommandID());
                    switch (pconfig->getLastCommandID())
                    {
                        case UC_MATRIX:
                            changeState(DS_MATRIX);
                            break;

                        case UC_ALLWORDS:
                            changeState(DS_WORDS);
                            break;

                        case UC_NORMAL:
                            changeState(DS_CLOCK);
                            break;
                    }
                }
                break;

        }
    }
}

