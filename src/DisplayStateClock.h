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
        CE_UNKNOWN,
        CE_ITIS,
        CE_TIME,
        CE_WEEKDAY,
        CE_DATE,
        CE_PULSE,
        CE_SPECIAL,
    };

public:
    DisplayStateClock() 
        : DisplayStateBase("clock") 
    {
        m_nPreviousMinute = -1;
        m_fShowBirthday = false;
        m_fShowHoliday = false;
    }

    virtual void    Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager);
    virtual bool    HandleLoop(unsigned long epochTime, time_t localTime);      
    virtual void    commandHandler(std::string command, std::vector<std::string> args);

protected:
    CRGB    ColorHandler(CRGB defaultColor, int brightness, int customParam = 0);

    CRGB    GetDisplayOptionsColor(int colorIndex, int optionIndex, std::minstd_rand0& generator);
    int     GetSeed(const EDisplayOptions eOption);

private:
    int                 m_nPreviousMinute;
    int                 m_nBrightness;
    int                 m_nPreviousBrightness;
    int                 m_nSeconds;
    int                 m_nMinutes;
    int                 m_nHours;
    int                 m_nWeekDay;
    bool                m_fShowBirthday;
    bool                m_fShowHoliday;
    std::minstd_rand0   m_randomTime;
    std::minstd_rand0   m_randomWeekday;
    std::minstd_rand0   m_randomDate;
};
