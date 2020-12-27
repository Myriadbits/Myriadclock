///////////////////////////////////////////////////////////////////////////////
// DisplayStateBooting class
// Show booting intro
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateBooting.h"
#include "MyriadclockConfig.h"

//
// Initialize
//
void DisplayStateBooting::Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings)
{
    DisplayStateBase::Initialize(pLEDs, pTZ, pSettings);
    m_timeStamp = millis();
    m_nCounter = 0;
    m_nCycleCounter = 0;
    m_nDir = 1;
}

//
// Loop
//
bool DisplayStateBooting::HandleLoop(unsigned long epochTime)
{
    CRGB colTop = CRGB(0, m_nCounter, 0);
   // CRGB colVersion = CRGB(0, m_nCounter/4, 0);

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
            m_nCycleCounter++;
        }
        
        FastLED.clear();

        // Set the background color (if required)
        FillBackground();

        // Show the myriadclock text
        AddWordToLeds(s_layout.extra.myriadclock, colTop);   

        // Show the codes
   //     AddWordToLeds((ledpos_t*) s_wordCodes[m_pSettings->nSerialNumber % 32].leds, colVersion); TODO
        // Show the version
        //const ledpos_t* pNumber = s_wordsMonthDays[(FIRMWARE_VERSION - 1) % 31]; 
        //AddWordToLeds((ledpos_t*) pNumber, colVersion);    

        FastLED.show();   
    }

    return (m_nCycleCounter < 4);
}
