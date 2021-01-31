///////////////////////////////////////////////////////////////////////////////
// DisplayStateToilet class
// Show hide toilet indication
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateToilet.h"


uint8_t DisplayStateToilet::s_toiletSign[16*16] = {
    0, 0, 91, 158, 91, 0, 0,            0, 0,   0, 0, 91, 180, 91, 0, 0,
    0, 0, 159, 173, 159, 0, 0,          0, 0,   0, 0, 180, 200, 180, 0, 0,
    0, 0, 91, 158, 91, 0, 0,            0, 0,   0, 0, 91, 180, 91, 0, 0,
    0, 0, 0, 45, 0, 0, 0,               0, 0,   0, 0, 0, 45, 0, 0, 0,
    0, 65, 146, 173, 146, 65, 0,        0, 0,   59, 156, 200, 200, 200, 156, 59, 
    66, 179, 182, 182, 182, 179, 66,    0, 0,   158, 114, 200, 200, 200, 114, 158,
    150, 141, 184, 184, 184, 141, 150,  0, 0,   200, 40, 200, 200, 200, 40, 200,
    184, 75, 187, 187, 187, 75, 184,    0, 0,   200, 40, 200, 200, 200, 40, 200,
    95, 49, 190, 190, 190, 49, 95,      0, 0,   200, 40, 200, 200, 200, 40, 200,
    0, 121, 193, 193, 193, 121, 0,      0, 0,   0, 40, 200, 200, 200, 40, 0, 
    25, 196, 196, 196, 196, 196, 25,    0, 0,   0, 40, 200, 200, 200, 40, 0,
    125, 199, 199, 199, 199, 199, 125,  0, 0,   0, 40, 200, 0, 200, 40, 0,
    0, 51, 202, 0, 202, 51, 0,          0, 0,   0, 40, 200, 0, 200, 40, 0,
    0, 53, 210, 0, 210, 53, 0,          0, 0,   0, 40, 200, 0, 200, 40, 0,
    0, 56, 222, 0, 222, 56, 0,          0, 0,   0, 40, 200, 0, 200, 40, 0,
    0, 0, 0, 0, 0, 0, 0,                0, 0,   0, 0, 0, 0, 0, 0, 0,
};

//
// Initialize
//
void DisplayStateToilet::Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager)
{
    DisplayStateBase::Initialize(pLEDs, pConfig, pManager);
    m_nWordIndexTop = 0;
    m_nWordIndexBottom = 0;
}

//
// Loop
//
bool DisplayStateToilet::HandleLoop(unsigned long epochTime, time_t localTime)
{
    //CRGB colTop = CRGB(0, 255, 0);
    //CRGB colBottom = CRGB(255, 0, 0);

    CRGB colTime = CRGB(m_pConfig->getConfigValue(CONFIG_COLOR_TIME));
    int r = colTime.r;
    int g = colTime.g;
    int b = colTime.b;

    if (Elapsed(m_timeStamp) > 500)
    {
        m_timeStamp = millis();

        FastLED.clear();

        int factor = 512;
        for(int y = 0; y < 16; y++)
        {
            for(int x = 0; x < 16; x++)
            {
                int index = (y * 16) + x;
                m_pLEDs[CalcLedPos(x, y)] = CRGB((r * s_toiletSign[index]) / factor, (g * s_toiletSign[index]) / factor, (b * s_toiletSign[index]) / factor);
            }
        }

        FastLED.show();   
    }
    return true; // State has no end
}
