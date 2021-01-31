///////////////////////////////////////////////////////////////////////////////
// DisplayStateUpdating class
// Update using fota
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateUpdating.h"
#include "MyriadclockConfig.h"

//
// Initialize
//
void DisplayStateUpdating::Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager)
{
    DisplayStateBase::Initialize(pLEDs, pConfig, pManager);
    m_timeStamp = millis();
}

//
// Loop
//
bool DisplayStateUpdating::HandleLoop(unsigned long epochTime, time_t localTime)
{
    FastLED.clear();
    FastLED.setBrightness(50);
    AddWordToLeds((ledpos_t*) s_layout.extra.myriadclock, CRGB(0x00, 0x00, 0x7F), 255);                           
    FastLED.show();   

    // Create new FOTA object 
    m_pesp32FOTA = new esp32FOTA(FOTA_NAME, FOTA_VERSION);
    if (m_pesp32FOTA != NULL)
    {
        m_pesp32FOTA->checkURL = FOTA_URL; // Set the URL to check for FOTA update
        if (m_pesp32FOTA->execHTTPcheck())
        {
            m_pesp32FOTA->execOTA();
        }
        delete m_pesp32FOTA;
    }
    return false; // Switch back to next state
}
