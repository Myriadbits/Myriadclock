#ifndef   __DISPLAYSTATEBOOTING_H__
#define   __DISPLAYSTATEBOOTING_H__

#include "Esp.h"
#include "DisplayStateBase.h"

///////////////////////////////////////////////////////////////////////////////
// Booting display state
///////////////////////////////////////////////////////////////////////////////
class DisplayStateBooting : public DisplayStateBase
{
public:
    void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    bool HandleLoop(unsigned long epochTime);  

private:
    int             m_nCounter;
    int             m_nDir;
    int             m_nCycleCounter;
};

#endif //__DISPLAYSTATEBOOTING_H__
