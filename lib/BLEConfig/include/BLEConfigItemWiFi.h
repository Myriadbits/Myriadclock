#pragma once

#include <string>
#include <vector>
#include "BLEConfigItemOption.h"


///////////////////////////////////////////////////////////////////////////////
// Single MIOT Configuration item
///////////////////////////////////////////////////////////////////////////////
class BLEConfigItemWiFi : public BLEConfigItemOption
{
public:
    BLEConfigItemWiFi(uint16_t id, const std::string name, bool secure = true, const std::string synopsis = std::string())
        : BLEConfigItemOption(id, EConfigType::CT_WIFI, name, 0, secure, synopsis)
    {        
    }

    std::string getSSID() { return m_sSSID; }
    std::string getPassphrase() { return m_sPassphrase; }


protected:
    virtual void onDecodeData(std::string data);
    virtual void onLoad(Preferences &preferences, char* pkey);
    virtual void onStore(Preferences &preferences, char* pkey);   
    virtual std::string valueToString();

private:
    void addWiFiSSIDOptions();
   
private:
    std::string m_sSSID;
    std::string m_sPassphrase;
};
