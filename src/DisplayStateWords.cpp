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
    m_nWordIndexDay = 0;
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
    // List all words 
    static const ledpos_t* s_wordsTop[] = {
        s_pLEDTime->leadtext, 
        s_pLEDTime->minute_1, s_pLEDTime->minute_2, s_pLEDTime->minute_3, s_pLEDTime->minute_4, s_pLEDTime->minute_5, 
        s_pLEDTime->minute_6, s_pLEDTime->minute_7, s_pLEDTime->minute_8, s_pLEDTime->minute_9, s_pLEDTime->minute_10, 
        s_pLEDTime->minute_11, s_pLEDTime->minute_12, s_pLEDTime->minute_13, s_pLEDTime->minute_14, s_pLEDTime->quarter,
        s_pLEDTime->to, s_pLEDTime->past, s_pLEDTime->half,
        s_pLEDTime->hours[0], s_pLEDTime->hours[1], s_pLEDTime->hours[2], s_pLEDTime->hours[3], s_pLEDTime->hours[4], 
        s_pLEDTime->hours[5], s_pLEDTime->hours[6], s_pLEDTime->hours[7], s_pLEDTime->hours[8], s_pLEDTime->hours[9],
        s_pLEDTime->hours[10], s_pLEDTime->hours[11], 
        s_pLEDTime->hour_full,        
        NULL };

    static const ledpos_t* s_wordsBottom[] = {   
        s_pLEDDate->weekdays[0], s_pLEDDate->weekdays[1], s_pLEDDate->weekdays[2], s_pLEDDate->weekdays[3], s_pLEDDate->weekdays[4], 
        s_pLEDDate->weekdays[5], s_pLEDDate->weekdays[6],
        s_pLEDDate->months[0], s_pLEDDate->months[1], s_pLEDDate->months[2], s_pLEDDate->months[3],
        s_pLEDDate->months[4], s_pLEDDate->months[5], s_pLEDDate->months[6], s_pLEDDate->months[7],
        s_pLEDDate->months[8], s_pLEDDate->months[9], s_pLEDDate->months[10], s_pLEDDate->months[11],
        s_pLEDExtra->birthday, s_pLEDExtra->bluetooth, s_pLEDExtra->holiday, s_pLEDExtra->myriadbits, 
        s_pLEDExtra->myriadclock, s_pLEDExtra->no, s_pLEDExtra->party, s_pLEDExtra->passcode, 
        s_pLEDExtra->weddingday, s_pLEDExtra->wifi,
        NULL };


    CRGB colTop =    CRGB(0, 255, 0);
    CRGB colCenter = CRGB(0, 0, 150);
    CRGB colBottom = CRGB(255, 0, 0);
    
    if (Elapsed(m_timeStamp) > 500)
    {
        ledpos_t* pCurrentWord = NULL;
        
        m_timeStamp = millis();

        FastLED.clear();

        do
        {
            pCurrentWord = (ledpos_t*) s_wordsTop[m_nWordIndexTop];
            m_nWordIndexTop++;
            if (pCurrentWord == NULL)
            {
                m_nWordIndexTop = 0;
                break;
            }
        } while (pCurrentWord == EMPTY);
        
        if (pCurrentWord != NULL)
            AddWordToLeds(pCurrentWord, colTop);                       

        do
        {
            pCurrentWord = (ledpos_t*) s_wordsBottom[m_nWordIndexBottom];
            m_nWordIndexBottom++;
            if (pCurrentWord == NULL)
            {
                m_nWordIndexBottom = 0;
                break;
            }
        } while (pCurrentWord == EMPTY);

        if (pCurrentWord != NULL)
            AddWordToLeds(pCurrentWord, colBottom);                       

        
        AddWordToLeds((ledpos_t*) s_pLEDDate->days[m_nWordIndexDay % 31], colCenter);                       
        m_nWordIndexDay++;

        FastLED.show();   
    }
    return true;
}
