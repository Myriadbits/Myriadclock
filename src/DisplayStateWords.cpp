///////////////////////////////////////////////////////////////////////////////
// DisplayStateWords class
// Show all words one after another
///////////////////////////////////////////////////////////////////////////////

#include "DisplayStateWords.h"

//
// Initialize
//
void DisplayStateWords::Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings)
{
    DisplayStateBase::Initialize(pLEDs, pTZ, pSettings);
    m_nWordIndexTop = 0;
    m_nWordIndexBottom = 0;
    strcpy(m_sCommand, "words");
    strcpy(m_sCommandDescription, "Show all words after eachother to validate the words");
}


void DisplayStateWords::CommandHandler(int argc, char *argv[])
{
}

//
// Loop
//
bool DisplayStateWords::HandleLoop(unsigned long epochTime)
{
    CRGB colTop = CRGB(0, 255, 0);
    CRGB colBottom = CRGB(255, 0, 0);
    
    if (Elapsed(m_timeStamp) > 500)
    {
        ledpos_t* pCurrentWord = NULL;
        
        m_timeStamp = millis();

        FastLED.clear();

        pCurrentWord = (ledpos_t*) s_wordsTop[m_nWordIndexTop];
        if (pCurrentWord == NULL)
        {
            m_nWordIndexTop = 0;
        }
        else
        {            
            m_nWordIndexTop++;
            AddWordToLeds(pCurrentWord, colTop);                       
        }

        pCurrentWord = (ledpos_t*) s_wordsBottom[m_nWordIndexBottom];
        if (pCurrentWord == NULL)
        {
            m_nWordIndexBottom = 0;
        }
        else
        {            
            m_nWordIndexBottom++;
            AddWordToLeds(pCurrentWord, colBottom);                       
        }

        FastLED.show();   
    }
    return true;
}
