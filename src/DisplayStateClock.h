#pragma once

#include "DisplayStateBase.h"
#include <random>
#include "ClockTimeWordConvertor.h"

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
        , m_nPreviousMinute{ 0 }
        , m_nBrightness{ 0 }
        , m_nPreviousBrightness{ 0 }
        , m_nSeconds{ 0 }
        , m_nMinutes{ 0 }
        , m_nHours{ 0 }
        , m_nWeekDay{ 0 }
        , m_nTransitionStep{ 0 }
        , m_fShowBirthday{ false }
        , m_fShowHoliday{ false }
        , m_fMinutesMainInTrans{ false }
        , m_fMinutesRestInTrans{ false }
        , m_fToPastInTrans{ false }
        , m_fHalfWordInTrans{ false }
        , m_fHourWordInTrans{ false }
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
    int                 m_nTransitionStep;
    bool                m_fShowBirthday;
    bool                m_fShowHoliday;
    std::minstd_rand0   m_randomTime;
    std::minstd_rand0   m_randomWeekday;
    std::minstd_rand0   m_randomDate;
    ClockWords_t        m_sClockWordsNow;
    ClockWords_t        m_sClockWordsNext;
    bool                m_fMinutesMainInTrans;
    bool                m_fMinutesRestInTrans;
    bool                m_fToPastInTrans;
    bool                m_fHalfWordInTrans;
    bool                m_fHourWordInTrans;
};
