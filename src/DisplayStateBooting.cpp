///////////////////////////////////////////////////////////////////////////////
// DisplayStateBooting class
// Show booting intro
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateBooting.h"
#include "MyriadclockConfig.h"

//
// Initialize
//
void DisplayStateBooting::Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager)
{
    DisplayStateBase::Initialize(pLEDs, pConfig, pManager);
    m_timeStamp = millis();
    m_nCounter = 0;
    m_nCycleCounter = 0;
    m_nDir = 1;
}

//
// Loop
//
bool DisplayStateBooting::HandleLoop(unsigned long epochTime, time_t localTime)
{
    CRGB colTop = CRGB(218, 81, 0);
    CRGB colWord = CRGB(173, 62, 0);

   // CRGB colVersion = CRGB(0, m_nCounter/4, 0);

    if (Elapsed(m_timeStamp) > 25)
    {
        m_timeStamp = millis();
        m_nCounter += m_nDir * 6;
        if (m_nCounter > 200) 
        {
            m_nCounter = 200;
            m_nDir *= -1;
        }
        if (m_nCounter < 0)
        {
            m_nCounter = 0;
            m_nDir *= -1;
            m_nCycleCounter++;
        }
        
        int brightness = GetBrightness(epochTime);

        FastLED.clear();

        // Set the background color (if required)
        int brightnessBackground = m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_BACKGROUND);
        FillBackground(brightnessBackground);

        // Show the myriadclock text
        AddWordToLeds(s_layout.extra.myriadclock, colTop, brightness);   
        AddWordToLeds(s_layout.extra.word, colWord, brightness);   

        // Show the codes
   //     AddWordToLeds((ledpos_t*) s_wordCodes[m_pSettings->nSerialNumber % 32].leds, colVersion); TODO
        // Show the version
        //const ledpos_t* pNumber = s_wordsMonthDays[(FIRMWARE_VERSION - 1) % 31]; 
        //AddWordToLeds((ledpos_t*) pNumber, colVersion);    

        FastLED.show();   
    }

    return (m_nCycleCounter < 4);
}
