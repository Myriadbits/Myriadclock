#pragma once

#include "DisplayStateBase.h"
#include <random>

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
    DisplayStateClock() : DisplayStateBase("clock") 
    {
    }

    virtual void    Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    virtual bool    HandleLoop(unsigned long epochTime);      
    virtual void    commandHandler(std::string command, std::vector<std::string> args);

protected:
    CRGB    ColorHandler(CRGB defaultColor, int brightness, int customParam = 0);

    void    CalcSunriseSunset(unsigned long timestamp, float lat, float lon, float alt, unsigned long *sunrise, unsigned long *sunset);
    void    UpdateBrightness(unsigned long epochTime);
    CRGB    GetDisplayOptionsColor(CRGB defaultColor, MyriadclockSettings::EDisplayOptions eOption, std::minstd_rand0& generator);

private:
    unsigned long   m_nPreviousEpochTime;
    int             m_nBrightness;
    int             m_nPreviousBrightness;
    int             m_nSeconds;
    int             m_nMinutes;
    int             m_nHours;
    int             m_nWeekDay;
    std::minstd_rand0 m_randomTime;
    std::minstd_rand0 m_randomWeekday;
    std::minstd_rand0 m_randomDate;
};
