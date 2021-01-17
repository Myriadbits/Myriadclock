#pragma once

#include "FastLED.h"
#include <Preferences.h>

enum EConfigType
{
    CT_WIFI_SSID,        // WiFi SSID
    CT_WIFI_PASSPHRASE,  // WiFi Passphrase
    CT_STRING,           // String
    CT_RGBCOLOR,         // Simple RGB color
    CT_SLIDER,           // Slider (0-100) 
    CT_UINT32,           // Value
};


///////////////////////////////////////////////////////////////////////////////
// Single MIOT Configuration item
///////////////////////////////////////////////////////////////////////////////
class MIOTConfigItem
{
public:
    MIOTConfigItem(uint16_t id, EConfigType type, std::string name, std::string synopsis, std::string unit);

    uint16_t getId() { return m_id;}
    EConfigType getType() { return m_eType;}
    std::string getName() { return m_sName;}    

    void setValue(uint32_t newValue) { m_value = newValue; }
    void setValue(std::string newValue) { m_valueString = newValue; }
    uint32_t getValue() { return m_value;}
    std::string getValueString();

    uint8_t encode(uint8_t *pdata, int dataLen);
    bool decode(std::string data, uint8_t *pdata);

private:
    uint16_t        m_id; // Unique ID 
    EConfigType     m_eType; // The configuratio item type
    std::string     m_sName; // Short name of this config item
    std::string     m_sSynopsis; // Short description
    std::string     m_sUnit; // Unit
    uint32_t        m_value;
    std::string     m_valueString;
};
