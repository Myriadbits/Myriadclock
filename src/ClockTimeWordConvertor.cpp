///////////////////////////////////////////////////////////////////////////////
// ClockTimeWordConvertor class
// Class to convert the time to words
///////////////////////////////////////////////////////////////////////////////

#include "ClockTimeWordConvertor.h"
#include "esp.h"
#include "ClockLayoutNL_V1.h"
#include "ClockLayoutNL_V2.h"
#include <Timezone.h>

#define arraysize(x)          (int)(sizeof((x))/sizeof(*(x)))

using namespace std;

//
// @brief  Start the console
//
void ClockTimeWordConvertor::convert(time_t localTime, ledclocklayout_t* pLayout, ClockWords_t* pOutput)
{
    // Convert to local time
    time_t t = localTime;

    int nWeekDay = weekday(t) - 1; // Weekday returns (1 - 7), Sunday = 1
    int nSeconds = second(t);
    int nHours = hour(t);
    int nMinutes = minute(t);        
    int monthday = day(t);
    int monthnum = month(t) - 1; // Januari = 1, we need it to be 0

    // Quarter past 1 => 14 minutes to half two (in Dutch this is correct, English I don't know)
    int min5 = nMinutes / 5;
    int min1 = nMinutes % 5;        
    int hours = nHours;

    // Clear the struct
    pOutput->pMinutesMainWord = NULL;
    pOutput->pMinutesRestWord = NULL;
    pOutput->pHalfWord = NULL;
    pOutput->pToPastWord = NULL;

    const ledtime_t* pTime = &(pLayout->time);

    switch (pLayout->timeFormat)
    {
    case ETimeFormat::TF_NL_EVERYMIN:
        {
            pOutput->pMinutesMainWord = pTime->leadtext;

            if (min5 > 3) hours++; // Increase the hour, but do NOT increase when we are at exactly a quarter past
            hours %= 12; // Limit hours to 12

            // Quarter past 1 => 14 minutes to half two (in Dutch this is correct, English I don't know)
            int quarterNum = nMinutes / 15;
            int min1 = nMinutes % 15;        
            int hours = nHours;
            if (quarterNum > 0 && !(quarterNum == 1 && min1 == 0)) hours++; // Increase the hour, but do NOT increase when we are at exactly a quarter past
            hours %= 12; // Limit hours to 12
            
            // Determine to/past
            if (min1 == 0)
            {
                // Specials quarters:
                switch (quarterNum)             
                {
                    case 0:                
                        pOutput->pMinutesRestWord = pTime->hour_full;
                        pOutput->pToPastWord = NULL; 
                        break;
                    case 1:                
                        pOutput->pMinutesRestWord = pTime->quarter;
                        pOutput->pToPastWord = pTime->past_15; 
                        break;
                    case 2:                
                        pOutput->pMinutesRestWord = pTime->half_to;
                        pOutput->pToPastWord = NULL; 
                        break;
                    case 3:                
                        pOutput->pMinutesRestWord = pTime->quarter;
                        pOutput->pToPastWord = pTime->to_15; 
                        break;
                }
            }
            else
            {
                pOutput->pToPastWord = pTime->past_5;
                if (quarterNum == 1 || quarterNum == 3)
                {
                    min1 = 15 - min1;
                    pOutput->pToPastWord = pTime->to_5;
                }
                if (nMinutes > 15 && nMinutes < 45)
                {
                    pOutput->pHalfWord =pTime-> half_to;
                }
                const ledpos_t* pMinuteWords[15] {NULL,pTime->minute_1, pTime->minute_2, pTime->minute_3, pTime->minute_4, 
                    pTime->minute_5, pTime->minute_6, pTime->minute_7, pTime->minute_8, pTime->minute_10, 
                    pTime->minute_11, pTime->minute_12, pTime->minute_13, pTime->minute_14};
                pOutput->pMinutesRestWord = pMinuteWords[min1 % 15];
            }               
        }
        break;
    
    case ETimeFormat::TF_NL_5MIN:
        {
            if (min5 > 3) hours++; // Increase the hour, but do NOT increase when we are at exactly a quarter past
            hours %= 12; // Limit hours to 12

            switch(min5)
            {
                case  0: pOutput->pMinutesMainWord = pTime->hour_full; break;
                case  1: pOutput->pMinutesMainWord = pTime->minute_5 ; pOutput->pToPastWord = pTime->past_5 ; break;
                case  2: pOutput->pMinutesMainWord = pTime->minute_10; pOutput->pToPastWord = pTime->past_10; break;
                case  3: pOutput->pMinutesMainWord = pTime->quarter;   pOutput->pToPastWord = pTime->past_15; break;
                case  4: pOutput->pMinutesMainWord = pTime->minute_10; pOutput->pToPastWord = pTime->to_10  ; pOutput->pHalfWord = pTime->half_to; break;
                case  5: pOutput->pMinutesMainWord = pTime->minute_5 ; pOutput->pToPastWord = pTime->to_5   ; pOutput->pHalfWord = pTime->half_to; break;
                case  6: pOutput->pMinutesMainWord = pTime->half_to; break;
                case  7: pOutput->pMinutesMainWord = pTime->minute_5 ; pOutput->pToPastWord = pTime->past_5 ; pOutput->pHalfWord = pTime->half_past; break;
                case  8: pOutput->pMinutesMainWord = pTime->minute_10; pOutput->pToPastWord = pTime->past_10; pOutput->pHalfWord = pTime->half_past; break;
                case  9: pOutput->pMinutesMainWord = pTime->quarter;   pOutput->pToPastWord = pTime->to_15  ; break;
                case 10: pOutput->pMinutesMainWord = pTime->minute_10; pOutput->pToPastWord = pTime->to_10  ; break;
                case 11: pOutput->pMinutesMainWord = pTime->minute_5 ; pOutput->pToPastWord = pTime->to_5   ; break;
            }
            
            const ledpos_t* pMinute5Words[5] {NULL, pTime->minute_1, pTime->minute_2, pTime->minute_3,pTime->minute_4};
            pOutput->pMinutesRestWord = pMinute5Words[min1];
        }
        break;

    case ETimeFormat::TF_EN_5MIN:
    default:
        {
            if (min5 > 6) hours++; // Increase the hour, when after the half
            hours %= 12; // Limit hours to 12

            switch(min5)
            {
                case  0: pOutput->pMinutesMainWord = pTime->hour_full; break;
                case  1: pOutput->pMinutesMainWord = pTime->minute_5 ; pOutput->pToPastWord = pTime->past_5 ; break;
                case  2: pOutput->pMinutesMainWord = pTime->minute_10; pOutput->pToPastWord = pTime->past_10; break;
                case  3: pOutput->pMinutesMainWord = pTime->quarter;   pOutput->pToPastWord = pTime->past_15; break;
                case  4: pOutput->pMinutesMainWord = pTime->minute_20; pOutput->pToPastWord = pTime->past_20; break;
                case  5: pOutput->pMinutesMainWord = pTime->minute_25; pOutput->pToPastWord = pTime->past_25; break;
                case  6: pOutput->pMinutesMainWord = pTime->half_past; break;
                case  7: pOutput->pMinutesMainWord = pTime->minute_25; pOutput->pToPastWord = pTime->to_25; break;
                case  8: pOutput->pMinutesMainWord = pTime->minute_20; pOutput->pToPastWord = pTime->to_20; break;
                case  9: pOutput->pMinutesMainWord = pTime->quarter;   pOutput->pToPastWord = pTime->to_15; break;
                case 10: pOutput->pMinutesMainWord = pTime->minute_10; pOutput->pToPastWord = pTime->to_10; break;
                case 11: pOutput->pMinutesMainWord = pTime->minute_5 ; pOutput->pToPastWord = pTime->to_5 ; break;
            }
            
            const ledpos_t* pMinute5Words[5] {NULL, pTime->minute_1, pTime->minute_2, pTime->minute_3,pTime->minute_4};
            pOutput->pMinutesRestWord = pMinute5Words[min1];
        }
        break;
    }

    pOutput->pHourWord = pLayout->hours[hours % 12];

    pOutput->pDayWord = pLayout->weekdays[nWeekDay % 7];
    pOutput->pDayOfMonthWord = pLayout->days[(monthday - 1) % 31]; // Days start at 1
    pOutput->pMonthWord = pLayout->months[monthnum % 12];
    pOutput->pSecondLeds = pTime->second;
}