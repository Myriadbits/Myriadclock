///////////////////////////////////////////////////////////////////////////////
// DisplayStateNoWiFi class
// Show noWifi intro
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateNoWiFi.h"
#include "MyriadclockConfig.h"

//
// Initialize
//
void DisplayStateNoWiFi::Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager)
{
    DisplayStateBase::Initialize(pLEDs, pConfig, pManager);
    m_timeStamp = millis();
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
        FillBackground();

        // Show the NoWiFi
        AddWordToLeds((ledpos_t*) s_layout.extra.no, colNoWiFi, 255);         
        AddWordToLeds((ledpos_t*) s_layout.extra.wifi, colNoWiFi, 255);         

        // Show the myriadclock text
        AddWordToLeds((ledpos_t*) s_layout.extra.myriadclock, colMyriadclock, 255);   

        // Show the codes
        //AddWordToLeds((ledpos_t*) s_wordCodes[m_pSettings->nSerialNumber % 32].leds, colMyriadclock);
        
        // Show the version
        //const ledpos_t* pNumber = s_wordsMonthDays[(FIRMWARE_VERSION - 1) % 31]; 
        //AddWordToLeds((ledpos_t*) pNumber, colVersion);    

        FastLED.show();   
    }

    return true; // This state has no end
}
