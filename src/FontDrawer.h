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
    FT_HIGHFONT48, // High font
    FT_HIGHFONT46, // High font small
    FT_582
};

enum ETextAlign
{
    TA_UNKNOWN = 0x00,
    TA_HCENTER = 0x01,
    TA_VCENTER = 0x02,
    TA_MIDTEXT = 0x04,
};

inline ETextAlign operator|(ETextAlign a, ETextAlign b)
{
    return static_cast<ETextAlign>(static_cast<int>(a) | static_cast<int>(b));
}

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

        void DrawGFX(CRGB *pLEDs, EFontType fontType, ETextAlign align, int x, int y, std::string text, CRGB color, int brightness);
        void log(const char* format, ...);

    private:
        int CalculateGFXWidth(const GFXfont* font, std::string text);
        int16_t LedPos(int8_t x, int8_t y);        
};
