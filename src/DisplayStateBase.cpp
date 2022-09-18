///////////////////////////////////////////////////////////////////////////////
// DisplayStateBase class
// Base class for controlling the display/led states
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateBase.h"


ledclocklayout_t DisplayStateBase::s_layout = s_layoutNL_V1;

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
// Set the clock layout
void DisplayStateBase::setLayout(const ledclocklayout_t *layout) 
{      
    memcpy(&s_layout, layout, sizeof(s_layout));    
};

//
// Color handler for words
CRGB DisplayStateBase::ColorHandler(CRGB defaultColor, int brightness, int customParam)
{
    return defaultColor.nscale8_video(brightness);
}

//
// Add a single word to the display/leds
// customParam can be any value, normal operation when ColorHandler is not overriden: customParam is the color
void DisplayStateBase::AddWordToLeds(const ledpos_t *pCurrentWord, CRGB defaultColor, int brightness, int customParam) 
{
    if (pCurrentWord == NULL) return;
    if (m_pLEDs == NULL) return;

    uint8_t charIndex = 0;
    ledpos_t ledPos = pCurrentWord[charIndex];
    while (ledPos.x >= 0 && ledPos.y >= 0)
    {
        // Led numbers are inverted left to right every other row:        
        m_pLEDs[CalcLedPos(ledPos.x, ledPos.y)] = ColorHandler(defaultColor, brightness, customParam);
        
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
    if (m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_BACKGROUND) != 0)
    {
        int brightness = m_pConfig->getConfigValue(CONFIG_BRIGHTNESS_BACKGROUND);
        CRGB colDefault = m_pConfig->getConfigValue(CONFIG_COLOR_BACKGROUND);

        CRGB rgbClear =  ColorHandler(colDefault, brightness, 0);
        for(int n = 0; n < NUM_LEDS; n++)
            m_pLEDs[n] = rgbClear;
    }
}

//
// Simple log function
void DisplayStateBase::log(const char* format, ...)
{
    static char sBuffer[256];
    va_list ap;
    va_start(ap, format);
    Serial.printf("[%s %lu] ", m_name.c_str(), millis());
    vsprintf(sBuffer, format, ap);
    Serial.println(sBuffer);
    va_end(ap);
}
