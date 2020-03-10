#pragma once

#include "DisplayStateBase.h"

///////////////////////////////////////////////////////////////////////////////
//
// Show the clock
//
///////////////////////////////////////////////////////////////////////////////
class DisplayStateClock : public DisplayStateBase
{
    enum EColorElement
    {
        CE_ITIS,
        CE_TIME,
        CE_WEEKDAY,
        CE_DATE,
        CE_PULSE,
    };

public:
    void    Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    bool    HandleLoop(unsigned long epochTime);  
    
protected:
    void    CalcSunriseSunset(unsigned long timestamp, float lat, float lon, float alt, unsigned long *sunrise, unsigned long *sunset);
    void    UpdateBrightness(unsigned long epochTime);
    CRGB    ColorHandler(uint32_t customParam);
    CRGB    GetDisplayOptionsColor(CRGB defaultColor, MyriadclockSettings::EDisplayOptions eOption);

private:
    unsigned long   m_nPreviousEpochTime;
    int             m_nPreviousBrightness;
    int             m_nSeconds;
    int             m_nMinutes;
    int             m_nHours;
    int             m_nWeekDay;
    std::random_device  m_random;
};
