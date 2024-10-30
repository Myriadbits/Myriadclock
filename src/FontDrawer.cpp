///////////////////////////////////////////////////////////////////////////////
// FontDrawer class
// Draws text on the LED panel
///////////////////////////////////////////////////////////////////////////////

#include "FontDrawer.h"
#include "MyriadclockConfig.h"
#include "esp.h"
#include <FastLED.h>
#include "Myriadbitsfont5x6_3.h"
#include "Myriadbitsfont5x8_3.h"
#include "HighFont4x8.h"
#include "HighFont4x6.h"

using namespace std;

#define NUM_COLS 32
#define NUM_ROWS 8

//
// @brief  Draw a character on the LED panel
//
void FontDrawer::DrawGFX(CRGB *pLEDs, EFontType fontType, ETextAlign align, int x, int y, std::string text, CRGB color, int brightness)
{
    const GFXfont* pFont = NULL;
    switch (fontType)
    {
        case EFontType::FT_56: pFont = &JochemFont56; break;
        case EFontType::FT_HIGHFONT48: pFont = &Highfont4x8; break;
        case EFontType::FT_HIGHFONT46: pFont = &Highfont4x6; break;
        case EFontType::FT_582: pFont = &JochemFont58; break;
    }
    if (pFont == NULL)
        return;

    int posX = 0;
    int posY = 0;

    if ((align & ETextAlign::TA_HCENTER) != 0)
    {
        int sizeX = CalculateGFXWidth(pFont, text);
        posX = (NUM_COLS - sizeX) / 2;
    }
    if ((align & ETextAlign::TA_VCENTER) != 0)
    {
        int sizeY = pFont->glyph->height;
        posY = (NUM_ROWS - sizeY) / 2;
    }
    if ((align & ETextAlign::TA_MIDTEXT) != 0)
    {
        int len = text.length();
        int sizeX = CalculateGFXWidth(pFont, text.substr(0, len/2));
        if ((len % 2) != 0)
        {
            // Uneven characters, so add half of the width of the center character
            sizeX += CalculateGFXWidth(pFont, text.substr(len/2 + 1, 1)) / 2;
        }
        posX = (NUM_COLS / 2) - sizeX;
    }

    CRGB colorBrightness = color.fadeLightBy(255 - brightness);
    for(char ch: text)
    {
        if (ch >= pFont->first && ch < pFont->last)
        {
            char c = (ch - pFont->first);

            GFXglyph *glyph = &(pFont->glyph[c]);
            uint8_t *bitmap = pFont->bitmap;

            uint16_t bo = glyph->bitmapOffset;
            uint8_t w = glyph->width;
            uint8_t h = glyph->height;

            uint8_t bits = 0;
            uint8_t bit = 0;
            for (int yy = h - 1; yy >= 0; yy--)
            {
                for (int xx = 0; xx < w; xx++)
                {
                    if (bit == 0x00)
                    {
                        bits = bitmap[bo++];
                        bit = 0x80;
                    }
                    if (bits & bit)
                    {
                        pLEDs[LedPos(posX + xx, posY + y + yy)] = colorBrightness;
                    }
                    bit = bit >> 1;
                }
            }
            posX += glyph->xAdvance;
        }
    }
}

/// @brief Calculate the width of an text
/// @param font 
/// @param text 
int FontDrawer::CalculateGFXWidth(const GFXfont* font, std::string text)
{
    int16_t xo16 = 0;
    for(char ch: text)
    {
        if (ch >= font->first && ch < font->last)
        {
            char c = (ch - font->first);
            GFXglyph *glyph = &(font->glyph[c]);
            xo16 += glyph->xAdvance;
        }
    }
    return xo16;
}


//
// Return the led index in the leds array
//
int16_t FontDrawer::LedPos(int8_t x, int8_t y)
{
    int pos = 0;
    if ((x % 2) == 0)
        pos = (NUM_ROWS - y - 1) + (x * NUM_ROWS); 
    else
        pos = y + (x * NUM_ROWS); 
    return (pos % (NUM_COLS * NUM_ROWS));
}

void FontDrawer::log(const char* format, ...)
{
    static char sBuffer[256];
    va_list ap;
    va_start(ap, format);
    Serial.printf("[FontDrawer %lu] ", millis());
    vsprintf(sBuffer, format, ap);
    Serial.println(sBuffer);
    va_end(ap);
}
