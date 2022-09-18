///////////////////////////////////////////////////////////////////////////////
// DisplayStateMatrix class
// Show the matrix
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateMatrix.h"

//
// Initialize
//
void DisplayStateMatrix::Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager)
{
    DisplayStateBase::Initialize(pLEDs, pConfig, pManager);
    m_nWordIndexTop = 0;
    m_nWordIndexBottom = 0;
    m_nWordIndexDay = 0;

    m_totalTime = 0;
    memset(m_introPos, 0, sizeof(m_introPos));
    memset(m_introLen, 0, sizeof(m_introLen));

    m_maxTotalTime = 30000; // X milliseconds total time (+ time it takes for beams to reach bottom)

    randomSeed(millis());        
}

//
// Loop
//
bool DisplayStateMatrix::HandleLoop(unsigned long epochTime, time_t localTime)
{
    if (Elapsed(m_timeStamp) >= 50)
    {
        m_timeStamp = millis();
        m_totalTime += 50; // Count the number of ms

        const ledtime_t* pTime = &(s_layout.time);

        // Convert to local time
        time_t t = localTime;
        int nHours = hour(t);
        int nMinutes = minute(t);

        int brightness = GetBrightness(epochTime);
        int val = (120 * brightness) >> 8;
        CRGB colLeading = CRGB(val, val, val);

        if (random(3) == 0)
        {
            uint8_t col = random(NUM_COLS);
            if (m_introPos[col] == 0 && (m_totalTime < m_maxTotalTime)) // Only allow new beam when not reaching the end of the steps
            {
                m_introPos[col] = 1;
                m_introLen[col] = 6 + random(14);
            }
        }

         // And now for the LEDS
        FastLED.clear();

        int beamCount = 0;
        for(uint8_t n = 0; n < NUM_COLS; n++)
        {
            uint8_t pos = m_introPos[n];
            if (pos != 0)
            {
                beamCount++;
                uint8_t len = m_introLen[n];                
                if (pos < NUM_ROWS)
                { 
                    // Leading char
                    m_pLEDs[CalcLedPos(n, pos)] = colLeading;
                }
                for(uint8_t i = 1; i < len; i++)
                {
                    if (pos - i >= 0 && pos - i < NUM_ROWS)
                    {
                        uint8_t fac = (80 / len);
                        m_pLEDs[CalcLedPos(n, pos - i)] = CRGB(0, ((80 - (i * fac)) * brightness) >> 8, 0);
                    }
                }

                m_introPos[n]++;
                if (m_introPos[n] - m_introLen[n] > NUM_ROWS)
                {
                    m_introPos[n] = 0;
                }
            }
        }

        if (nHours == 0 && nMinutes == 0)
        {
            CRGB colTest = CRGB(255, 255, 255);
            AddWordToLeds(s_layout.hours[0], colTest, brightness);
            AddWordToLeds(pTime->hour_full, colTest, brightness);
        }                

        FastLED.show(); 

        if ((m_totalTime > (m_maxTotalTime + 5000)) && (beamCount == 0))
        {
            return false; // Quit state
        }
    }
    return true; 
}
