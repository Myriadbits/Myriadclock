///////////////////////////////////////////////////////////////////////////////
// DisplayStateNoWiFi class
// Show noWifi intro
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateNoWiFi.h"
#include "MyriadclockConfig.h"

#define UPDATE_SSID_TIME     120000 // Update the SSID list once every x milliseconds

//
// Initialize
//
void DisplayStateNoWiFi::Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager)
{
    DisplayStateBase::Initialize(pLEDs, pConfig, pManager);
    m_updateSSIDTimeStamp = millis();
    m_nCounter = 0;
    m_nDir = 1;
}

//
// Loop
//
bool DisplayStateNoWiFi::HandleLoop(unsigned long epochTime, time_t localTime)
{
    CRGB colNoWiFi = CRGB(m_nCounter, 0, 0);
    CRGB colMyriadclock = CRGB(0, 128, 0);
    CRGB colBluetooth = CRGB(75, 144, 255);
    //CRGB colVersion = CRGB(0, m_nCounter/4, 0);

    if (Elapsed(m_timeStamp) > 25)
    {
        m_timeStamp = millis();
        m_nCounter += m_nDir * 8;
        if (m_nCounter > 255) 
        {
            m_nCounter = 255;
            m_nDir *= -1;
        }
        if (m_nCounter < 0)
        {
            m_nCounter = 0;
            m_nDir *= -1;
        }
        
        FastLED.clear();

        // Set the background color (if required)
        int brightnessBackground = m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_BACKGROUND);
        FillBackground(brightnessBackground);
                
        int brightnessDay = m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_DAY);
        if (brightnessDay < 3) // Make sure there is a minimum
            brightnessDay = 3;

        // Show the NoWiFi
        AddWordToLeds((ledpos_t*) s_layout.extra.no, colNoWiFi, brightnessDay);
        AddWordToLeds((ledpos_t*) s_layout.extra.wifi, colNoWiFi, brightnessDay);         

        // Show the myriadclock text
        AddWordToLeds((ledpos_t*) s_layout.extra.myriadclock, colMyriadclock, brightnessDay);   

        // Show the myriadclock text
        if (m_pConfig->isDeviceConnected())
        {
            AddWordToLeds((ledpos_t*) s_layout.extra.bluetooth, colBluetooth, brightnessDay);
        }

        // Show the codes
        //AddWordToLeds((ledpos_t*) s_wordCodes[m_pSettings->nSerialNumber % 32].leds, colMyriadclock);
        
        // Show the version
        //const ledpos_t* pNumber = s_wordsMonthDays[(FIRMWARE_VERSION - 1) % 31]; 
        //AddWordToLeds((ledpos_t*) pNumber, colVersion);    

        FastLED.show();   
    }


    // Once every 10 seconds, update the WiFi networks
    if (Elapsed(m_updateSSIDTimeStamp) > UPDATE_SSID_TIME)
    {
        m_updateSSIDTimeStamp = millis();

        // If there is a WiFi config item: update the WIFi config item
        if (m_pConfig != NULL)
        {
            BLEConfigItemWiFi* pciWiFi = reinterpret_cast<BLEConfigItemWiFi*>(m_pConfig->getConfigItem(CONFIG_WIFI));
            if (pciWiFi != NULL)
            {
                pciWiFi->refreshSSIDList();
            }
        }
    }

    return true; // This state has no end
}
