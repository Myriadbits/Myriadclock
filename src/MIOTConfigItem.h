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
    MIOTConfigItem(uint32_t id, EConfigType type, std::string name, std::string synopsis)        
        : m_id(id)
        , m_eType(type)
        , m_sName(name)
        , m_sSynopsis(synopsis)
    {        
    }

    uint32_t        m_id;
    EConfigType     m_eType; // The configuratio item type
    std::string     m_sName; // Name of this config item
    std::string     m_sSynopsis; // Short description
    uint32_t        m_value;
};
