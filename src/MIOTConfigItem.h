#pragma once

#include <string>
#include <vector>

enum EConfigType
{
    CT_UNKNOWN,            
    CT_STRING,           // String
    CT_UINT32,           // Value    
    CT_RGBCOLOR,         // RGB color
    CT_SLIDER,           // Slider (0-100) 
    CT_OPTION,           // Options (combobox/selection box)
    CT_DATE,             // Date
    CT_TIME,             // Time
    CT_WIFI,             // List of all WiFi SSIDS in range (CT_OPTION with SSIDs filled in), when switching, we expect the passphrase as well
};

typedef struct 
{
    uint8_t         m_value; // The value of the config option
    std::string     m_sName; // Name of this option item
} MIOTConfigOption_t;


///////////////////////////////////////////////////////////////////////////////
// Single MIOT Configuration item
///////////////////////////////////////////////////////////////////////////////
class MIOTConfigItem
{
public:
    MIOTConfigItem(const uint16_t id, const EConfigType type, const std::string name, const bool secure, const std::string synopsis);

    uint16_t getId() { return m_id;}
    EConfigType getType() { return m_eType;}
    std::string getName() { return m_sName;}    

    void setValue(const uint32_t newValue) { m_value = newValue; }
    void setValue(const std::string newValue) { m_valueString = newValue; }
    
    uint32_t getValue() { return m_value;}
    std::string getValueString();

    bool isSecure() { return m_fSecure;}

    void clearOptions();
    void addOption(const uint8_t optionValue, const std::string optionText);
    MIOTConfigOption_t* getOption(const uint8_t optionValue);

    uint8_t encode(uint8_t *pdata, int dataLen);
    bool decode(std::string data);//, uint8_t *pdata);

private:
    uint16_t        m_id; // Unique ID 
    EConfigType     m_eType; // The configuratio item type
    std::string     m_sName; // Short name of this config item
    std::string     m_sSynopsis; // Short description
    uint32_t        m_value;
    std::string     m_valueString;
    bool            m_fSecure; // This config item requires a secure connection
    std::vector<MIOTConfigOption_t> m_vecOptions;
};
