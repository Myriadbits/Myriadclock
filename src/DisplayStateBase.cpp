///////////////////////////////////////////////////////////////////////////////
// DisplayStateBase class
// Base class for controlling the display/led states
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateBase.h"

//
//  Return the elapsed time in milliseconds since a timestamp
//
uint32_t DisplayStateBase::Elapsed(uint32_t ts)
{
    uint32_t now = millis();
    if (ts < now) return (now - ts);
    return(now + (0xffffffff - ts));
}

//
// Return the led index in the leds array
//
int16_t DisplayStateBase::CalcLedPos(int8_t x, int8_t y)
{
    if ((y % 2) == 0)
        return (NUM_COLS - x - 1) + (y * NUM_COLS); 
    else
        return x + (y * NUM_COLS); 
}

//
// Color handler for words
CRGB DisplayStateBase::ColorHandler(uint32_t customParam)
{
    return CRGB(customParam);
}

//
// Add a single word to the display/leds
// customParam can be any value, normal operation when ColorHandler is not overriden: customParam is the color
void DisplayStateBase::AddWordToLeds(ledpos_t* pCurrentWord, uint32_t customParam) 
{
    if (pCurrentWord == NULL) return;
    if (m_pLEDs == NULL) return;

    uint8_t charIndex = 0;
    ledpos_t ledPos = pCurrentWord[charIndex];
    while (ledPos.x >= 0 && ledPos.y >= 0)
    {
        // Led numbers are inverted left to right every other row:        
        m_pLEDs[CalcLedPos(ledPos.x, ledPos.y)] = ColorHandler(customParam);
        
        // Next char
        charIndex++;
        ledPos = pCurrentWord[charIndex];
    }
}

//
// Fill the entire background with a color
void DisplayStateBase::FillBackground(void)
{
    // Set the background color
    if (m_pSettings->nBrightnessBackground != 0)
    {
        CRGB rgbBackground = m_pSettings->colBackground;
        rgbBackground.fadeToBlackBy( 255 - (255 * m_pSettings->nBrightnessBackground) / 100 );
        for(int n = 0; n < NUM_LEDS; n++)
            m_pLEDs[n] = rgbBackground;
    }
}


