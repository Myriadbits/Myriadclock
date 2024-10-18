#pragma once

#include <vector>
#include <string>
#include <FastLED.h>
#include "FontsBase.h"

#define COMMAND_BUFFER_SIZE     256 // Maximum # chars on a line

#ifndef NULL
#define NULL    (void *)0
#endif

enum EFontType
{
    FT_UNKNOWN,
    FT_56,
    FT_48,
    FT_582
};


enum ETextAlign
{
    TA_UNKNOWN,
    TA_HCENTER,
    TA_MIDTEXT,
};

///////////////////////////////////////////////////////////////////////////////
// Console class
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class FontDrawer
{
    public:
        FontDrawer()
        {
        }

        FontDrawer(FontDrawer const&) = delete;
        void operator=(FontDrawer const&) = delete;

        static FontDrawer& getInstance()
        {
            static FontDrawer instance; // Guaranteed to be destroyed. Instantiated on first use.
            return instance;
        }

        void Draw(CRGB *pLEDs, int x, int y, std::string text, CRGB color, int brightness);
        void DrawGFX(CRGB *pLEDs, EFontType fontType, ETextAlign align, int x, int y, std::string text, CRGB color, int brightness);
        void log(const char* format, ...);

    private:
        int CalculateWidth(uint8_t* pFont, std::string text);
        int CalculateGFXWidth(const GFXfont* font, std::string text);
        int16_t LedPos(int8_t x, int8_t y);        
};
