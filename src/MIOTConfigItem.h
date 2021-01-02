#pragma once

#include "FastLED.h"
#include <Preferences.h>


enum EConfigType
{
    CT_WIFI_SSID,           // WiFi SSID
    CT_WIFI_PASSWORD,       // Password for WiFi
    CT_RGBCOLOR,            // Simple RGB color
    CT_SLIDER,              // Slider (0-100) 
};

///////////////////////////////////////////////////////////////////////////////
// Single MIOT Configuration item
///////////////////////////////////////////////////////////////////////////////
class MIOTConfigItem
{
public:
    MIOTConfigItem(uint8_t id, EConfigType type, std::string name, std::string synopsis, std::string unit);

    uint8_t getId() { return m_id;}
    EConfigType getType() { return m_eType;}
    std::string getName() { return m_sName;}    

    void setValue(uint32_t newValue) { m_value = newValue; }
    uint32_t getValue() { return m_value;}

    uint8_t* encode(uint8_t* outputLen);

private:
    uint8_t         m_id; // Unique ID 
    EConfigType     m_eType; // The configuratio item type
    std::string     m_sName; // Short name of this config item
    std::string     m_sSynopsis; // Short description
    std::string     m_sUnit; // Unit
    uint32_t        m_value;
};
