#ifndef   __DISPLAYSTATEUPDATING_H__
#define   __DISPLAYSTATEUPDATING_H__

#include "Esp.h"
#include "DisplayStateBase.h"
#include "esp32fota.h"

// Defines
//#define FOTA_NAME           "Myriadclock-fota"
//#define FOTA_VERSION        1
//#define FOTA_URL            "https://www.myriadbits.com/Myriadclock-fota.json"

///////////////////////////////////////////////////////////////////////////////
// Updating display state
///////////////////////////////////////////////////////////////////////////////
class DisplayStateUpdating : public DisplayStateBase
{
public:
    void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    bool HandleLoop(unsigned long epochTime);  

private:
    esp32FOTA*       m_pesp32FOTA; //("Myriadclock-fota", 1);
};

#endif //__DISPLAYSTATEUPDATING_H__
