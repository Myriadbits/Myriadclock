///////////////////////////////////////////////////////////////////////////////
// DisplayStateUpdating class
// Update using fota
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateUpdating.h"
#include "MyriadclockConfig.h"

//
// Initialize
//
void DisplayStateUpdating::Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings)
{
    DisplayStateBase::Initialize(pLEDs, pTZ, pSettings);
    m_timeStamp = millis();
    strcpy(m_sCommand, "update");
    strcpy(m_sCommandDescription, "Update using FOTA");   
}

//
// Loop
//
bool DisplayStateUpdating::HandleLoop(unsigned long epochTime)
{
    FastLED.clear();
    FastLED.setBrightness(50);
    AddWordToLeds((ledpos_t*) myriadbits, CRGB(0x00, 0x00, 0x7F));                           
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
