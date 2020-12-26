#pragma once

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
    DisplayStateUpdating() : DisplayStateBase("updating") 
    {
    }

    virtual void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    virtual bool HandleLoop(unsigned long epochTime);  

private:
    esp32FOTA*       m_pesp32FOTA; //("Myriadclock-fota", 1);
};

