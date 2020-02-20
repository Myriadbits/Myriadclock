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
// Add a single word to the display/leds
//
void DisplayStateBase::AddWordToLeds(ledpos_t* pCurrentWord, CRGB rgbColor) 
{
    if (pCurrentWord == NULL) return;

    uint8_t charIndex = 0;
    do
    {
        ledpos_t ledPos = pCurrentWord[charIndex];
        if (ledPos.x < 0 || ledPos.y < 0)
            break;

        // Led numbers are inverted left to right every other row:        
        int16_t ledIndex = CalcLedPos(ledPos.x, ledPos.y); 
        if (m_pLEDs != NULL)
            m_pLEDs[ledIndex] = rgbColor;
        charIndex++;
    }
    while (true);
}
