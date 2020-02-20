///////////////////////////////////////////////////////////////////////////////
// DisplayStateBooting class
// Show booting intro
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateBooting.h"
#include "MyriadclockConfig.h"

//
// Initialize
//
void DisplayStateBooting::Initialize(CRGB* pLEDs, Timezone* pTZ)
{
    DisplayStateBase::Initialize(pLEDs, pTZ);
    m_timeStamp = millis();
    m_nCounter = 0;
    m_nCycleCounter = 0;
    m_nDir = 1;
    strcpy(m_sCommand, "booting");
    strcpy(m_sCommandDescription, "Show the booting sequence");

    FastLED.clear();
    AddWordToLeds((ledpos_t*) nowifi, CRGB(0xFF, 0x7E, 0x00));                           
    FastLED.show();   
}

//
// Loop
//
bool DisplayStateBooting::HandleLoop(unsigned long epochTime)
{
    CRGB colTop = CRGB(0, m_nCounter, 0);
    CRGB colVersion = CRGB(0, m_nCounter/4, 0);

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

        AddWordToLeds((ledpos_t*) myriadclock, colTop);   

        const ledpos_t* pNumber = s_wordsMonthDays[(FIRMWARE_VERSION - 1) % 31]; 
        AddWordToLeds((ledpos_t*) pNumber, colVersion);    

        FastLED.show();   
    }

    return (m_nCycleCounter < 4);
}
