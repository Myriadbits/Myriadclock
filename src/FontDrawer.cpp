///////////////////////////////////////////////////////////////////////////////
// FontDrawer class
// Draws text on the LED panel
///////////////////////////////////////////////////////////////////////////////

#include "FontDrawer.h"
#include "MyriadclockConfig.h"
#include "esp.h"
#include "Fonts.h"
#include <FastLED.h>
#include "Myriadbitsfont5x6_3.h"
#include "Myriadbitsfont5x8_3.h"
#include "GFXHighFont4x8.h"

using namespace std;

#define NUM_COLS 32
#define NUM_ROWS 8

#include "Fonts.cpp"

//
// @brief  Draw a character on the LED panel
//
void FontDrawer::Draw(CRGB *pLEDs, int x, int y, std::string text, CRGB color, int brightness)
{
    int offset = x;
    uint8_t* pFont = CrystalFontz5x8;
    uint8_t fontWidth = pFont[2];
    uint8_t fontHeight = pFont[3];
    uint8_t firstChar = pFont[4];
    uint8_t charCount = pFont[5];

    CRGB colorBrightness = color.fadeLightBy(255 - brightness);

    int sizeX = CalculateWidth(pFont, text);
    int posX = (NUM_COLS - sizeX) / 2;
    for(char ch: text)
    {
        if (ch >= firstChar && ch < firstChar + charCount)
        {
            int dataIndex = ((int)(ch - firstChar) * fontWidth) + 6;
            for(int offsetX = 0; offsetX < fontWidth; offsetX++)
            {
                uint8_t fontColData = pFont[dataIndex + offsetX];
                uint8_t mask = 0x01;
                if (fontColData == 0x00 && ch != 32)
                    continue;

                for(int offsetY = fontHeight - 1; offsetY >= 0; offsetY--)
                {                    
                    if ((fontColData & mask) == mask)
                    {
                        pLEDs[LedPos(posX, offsetY)] = colorBrightness;
                    }
                    else
                    {
                        //pLEDs[LedPos(x, offsetY)] = 0x000030;
                    }
                    mask = mask << 0x01;
                }
                posX++;
            }
            posX++;
        }
    }
}

/// @brief Calculate the length of a piece of text in pixels
/// @param pFont 
/// @param text 
/// @return 
int FontDrawer::CalculateWidth(uint8_t* pFont, std::string text)
{
    int offset = 0;
    uint8_t fontWidth = pFont[2];
    uint8_t fontHeight = pFont[3];
    uint8_t firstChar = pFont[4];
    uint8_t charCount = pFont[5];

    int x = 0;
    for(char ch: text)
    {
        if (ch >= firstChar && ch < firstChar + charCount)
        {
            int dataIndex = ((int)(ch - firstChar) * fontWidth) + 6;
            for(int offsetX = 0; offsetX < fontWidth; offsetX++)
            {
                uint8_t fontColData = pFont[dataIndex + offsetX];
                uint8_t mask = 0x01;
                if (fontColData == 0x00 && ch != 32)
                    continue;                
                x++;
            }
            x++;
        }
    }
    return x - 1;
}

//
// @brief  Draw a character on the LED panel
//
void FontDrawer::DrawGFX(CRGB *pLEDs, EFontType fontType, ETextAlign align, int x, int y, std::string text, CRGB color, int brightness)
{
    const GFXfont* pFont = NULL;
    switch (fontType)
    {
        case EFontType::FT_56: pFont = &JochemFont56; break;
        case EFontType::FT_48: pFont = &Highfont4x8; break;
        case EFontType::FT_582: pFont = &JochemFont58; break;
    }
    if (pFont == NULL)
        return;

    int posX = 0;

    if (align == ETextAlign::TA_HCENTER)
    {
        int sizeX = CalculateGFXWidth(pFont, text);
        posX = (NUM_COLS - sizeX) / 2;
    }
    if (align == ETextAlign::TA_MIDTEXT)
    {
        int len = text.length();
        int sizeX = CalculateGFXWidth(pFont, text.substr(0, len/2));
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
                        pLEDs[LedPos(posX + xx, y + yy)] = colorBrightness;
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
