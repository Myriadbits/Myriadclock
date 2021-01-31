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

    virtual void Initialize(CRGB* pLEDs, BLEConfig* pConfig, DisplayStateManager *pManager);
    virtual bool HandleLoop(unsigned long epochTime, time_t localTime);  

private:
    esp32FOTA*       m_pesp32FOTA; //("Myriadclock-fota", 1);
};

