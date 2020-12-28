///////////////////////////////////////////////////////////////////////////////
// DisplayStatePasscode class
// Show bluetooth passcode
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStatePasscode.h"
#include "MyriadclockConfig.h"

using namespace std;

//
// Initialize
//
void DisplayStatePasscode::Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings)
{
    DisplayStateBase::Initialize(pLEDs, pTZ, pSettings);
    m_timeStamp = millis();
    if (pSettings != NULL)
    {
        char numberAsString[32];
        snprintf(numberAsString, 32, "%u", pSettings->bluetoothPasscode);
        m_codeAsString = string(numberAsString);
        log("Code as string: '%s'", m_codeAsString.c_str());
    }
    else
        log("Invalid settings object!");

    m_nCounter = 0;
    m_nSubCounter = 0;
    m_nDelay = 0;
    m_fShowResult = false;
    m_fResult = false;    
}

//
// Show hide the connection result
void DisplayStatePasscode::setResult(bool success)
{
    log("Passcode result = %d", success);
    m_fShowResult = true;
    m_fResult = success;
    m_nCounter = 0;
}

//
// Loop
//
bool DisplayStatePasscode::HandleLoop(unsigned long epochTime)
{
    CRGB colTextFirst = CRGB(0, 0xFF, 0);
    CRGB colTextMiddle = CRGB(0x7F, 0x7F, 0);
    CRGB colTextLast = CRGB(0xFF, 0, 0);
    CRGB colText = colTextFirst;
    CRGB colPasscode = CRGB(0xFF, 0x7F, 0);

    bool ret = true;
    if (Elapsed(m_timeStamp) > m_nDelay)
    {
        m_timeStamp = millis();
        
        FastLED.clear();

        // Set the background color (if required)
        FillBackground();

        if (m_fShowResult)
        {
            colPasscode = (m_fResult) ? CRGB(0, 0xFF, 0) : CRGB(0xFF, 0, 0);
            AddWordToLeds((ledpos_t*) s_layout.extra.bluetooth, colPasscode);
            AddWordToLeds((ledpos_t*) s_layout.extra.passcode, colPasscode);
            if (m_fResult)
                AddWordToLeds((ledpos_t*) s_layout.extra.yes, colPasscode);
            else
                AddWordToLeds((ledpos_t*) s_layout.extra.no, colPasscode);
            m_nDelay = 100;
            m_nCounter++;
            if (m_nCounter > 50)
                ret = false;
        }
        else
        {
            // Show the passcode
            AddWordToLeds((ledpos_t*) s_layout.extra.bluetooth, colPasscode);
            AddWordToLeds((ledpos_t*) s_layout.extra.passcode, colPasscode);

            // Subcounter is used to show/hide a single number
            // Show every number for half a second, pause between numbers of 200 ms
            if (m_codeAsString.length() > 1) // (Only when we have a code..)
            {
                if (m_nSubCounter == 0)
                {
                    int ch = m_codeAsString[m_nCounter];
                    log("Displaying: %s[%d] = '%d'", m_codeAsString.c_str(), m_nCounter, ch - 48);

                    // First char is green, middle other, last red
                    if (m_nCounter == 0) colText = colTextFirst;
                    else if (m_nCounter == m_codeAsString.length() - 1) colText = colTextLast;
                    else colText = colTextMiddle;

                    AddWordToLeds(s_layout.numbers[ch - 48], colText);
                    m_nDelay = (m_nCounter == 0) ? 3500 : 2500;
                }
                else if (m_nSubCounter == 1)
                {
                    m_nDelay = 500;
                }
                m_nSubCounter++;
                if (m_nSubCounter > 1)
                {
                    m_nSubCounter = 0;

                    // Next character please
                    m_nCounter++;
                    if (m_nCounter >= m_codeAsString.length())
                        m_nCounter = 0; // Restart
                }
            }
        }
        FastLED.show();   
    }
    return ret;
}
