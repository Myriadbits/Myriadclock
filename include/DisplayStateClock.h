#ifndef   __DISPLAYSTATECLOCK_H__
#define   __DISPLAYSTATECLOCK_H__

#include "Esp.h"
#include "DisplayStateBase.h"

///////////////////////////////////////////////////////////////////////////////
//
// Show the clock
//
///////////////////////////////////////////////////////////////////////////////
class DisplayStateClock : public DisplayStateBase
{
public:
    void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    bool HandleLoop(unsigned long epochTime);  
    
protected:
    void CalcSunriseSunset(unsigned long timestamp, float lat, float lon, float alt, unsigned long *sunrise, unsigned long *sunset);
private:
    unsigned long   m_nPreviousEpochTime;
    int             m_nPreviousBrightness;
};

#endif //__DISPLAYSTATECLOCK_H__
