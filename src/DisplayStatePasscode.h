#pragma once

#include "DisplayStateBase.h"

///////////////////////////////////////////////////////////////////////////////
// NoWiFi display state
///////////////////////////////////////////////////////////////////////////////
class DisplayStatePasscode : public DisplayStateBase
{
public:
    DisplayStatePasscode() : DisplayStateBase("passcode") 
    {
    }

    virtual void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    virtual bool HandleLoop(unsigned long epochTime);  

    void setResult(bool success);

private:
    int             m_nCounter;
    int             m_nSubCounter;
    int             m_nDelay;
    bool            m_fShowResult;
    bool            m_fResult;
    std::string     m_codeAsString;
};
