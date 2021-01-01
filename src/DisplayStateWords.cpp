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
}


//
// Loop
//
bool DisplayStateWords::HandleLoop(unsigned long epochTime)
{    
    // List all words 
    const ledpos_t* s_wordsTop[] = {
        s_layout.time.leadtext, 
        s_layout.time.minute_1,  s_layout.time.minute_2,  s_layout.time.minute_3,  s_layout.time.minute_4,  s_layout.time.minute_5, 
        s_layout.time.minute_6,  s_layout.time.minute_7,  s_layout.time.minute_8,  s_layout.time.minute_9,  s_layout.time.minute_10, 
        s_layout.time.minute_11,  s_layout.time.minute_12,  s_layout.time.minute_13,  s_layout.time.minute_14,  s_layout.time.quarter,
        s_layout.time.to_5,  s_layout.time.past_5,  s_layout.time.to_10,  s_layout.time.past_10,  s_layout.time.to_15,  s_layout.time.past_15, 
        s_layout.time.to_20,  s_layout.time.past_20,  s_layout.time.to_25,  s_layout.time.past_25,
        s_layout.time.half_past,  s_layout.time.half_to,
        s_layout.hours[0],  s_layout.hours[1],  s_layout.hours[2],  s_layout.hours[3],  s_layout.hours[4], 
        s_layout.hours[5],  s_layout.hours[6],  s_layout.hours[7],  s_layout.hours[8],  s_layout.hours[9],
        s_layout.hours[10],  s_layout.hours[11], 
        s_layout.time.hour_full,        
        NULL };

    const ledpos_t* s_wordsBottom[] = {   
        s_layout.weekdays[0], s_layout.weekdays[1], s_layout.weekdays[2], s_layout.weekdays[3], s_layout.weekdays[4], 
        s_layout.weekdays[5], s_layout.weekdays[6],
        s_layout.months[0], s_layout.months[1], s_layout.months[2], s_layout.months[3],
        s_layout.months[4], s_layout.months[5], s_layout.months[6], s_layout.months[7],
        s_layout.months[8], s_layout.months[9], s_layout.months[10], s_layout.months[11],
        s_layout.extra.birthday, s_layout.extra.bluetooth, s_layout.extra.holiday, s_layout.extra.myriadbits, 
        s_layout.extra.myriadclock, s_layout.extra.no, s_layout.extra.yes, s_layout.extra.party, s_layout.extra.passcode, 
        s_layout.extra.goodmorning, s_layout.extra.wakeup, s_layout.extra.wifi,
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
            AddWordToLeds(pCurrentWord, colTop, 255);                       

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
            AddWordToLeds(pCurrentWord, colBottom, 255);        

        
        AddWordToLeds((ledpos_t*) s_layout.days[m_nWordIndexDay % 31], colCenter, 255);                       
        m_nWordIndexDay++;

        FastLED.show();   
    }
    return true; // State has no end
}
