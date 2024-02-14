///////////////////////////////////////////////////////////////////////////////
// DisplayStateAnalog class
// Show an 'analog'clock
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateAnalog.h"

//
// Initialize
//
void DisplayStateAnalog::Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager)
{
    DisplayStateBase::Initialize(pLEDs, pConfig, pManager);
    m_nWordIndexTop = 0;
    m_nWordIndexBottom = 0;
    m_nWordIndexDay = 0;
    m_nLoopCounter = 0;
}

//
// Loop
//
bool DisplayStateAnalog::HandleLoop(unsigned long epochTime, time_t localTime)
{
    if (Elapsed(m_timeStamp) > 100)
    {
        m_timeStamp = millis();

        // Convert to local time
        time_t t = localTime;

        CRGB colDefault = m_pConfig->getConfigValue(CONFIG_COLOR_TIME);
        int brightnessBackground = m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_BACKGROUND);

        int nSeconds = second(t);
        int nHours = hour(t);
        int nMinutes = minute(t);        
        
        FastLED.clear();

        // Set the background color (if required)
        FillBackground(brightnessBackground);

        int brightness = GetBrightness(epochTime);

        int xpos = 0;
        int ypos = 0;
        if (nSeconds < 8) // 8 right (incl 0)
        {
            xpos = nSeconds + 8;
            ypos = 0;
        }
        else if (nSeconds < 22) // 14 down
        {
            xpos = 15;
            ypos = nSeconds - 7;
        }
        else if (nSeconds < 38) // 16 left
        {
            xpos = 37 - nSeconds;
            ypos = 15;
        }
        else if (nSeconds < 52) // 14 up
        {
            xpos = 0;
            ypos = 52 - nSeconds;
        }
        else if (nSeconds < 60) // 8 right
        {
            xpos = nSeconds - 52;
            ypos = 0;
        }

        m_pLEDs[CalcLedPos(xpos, ypos)] = ColorHandler(colDefault, brightness, 0);

        FastLED.show();   
    }
    //if (m_nLoopCounter >= 2)
    //    return false; // Quit after 2 loops
    return true;
}
