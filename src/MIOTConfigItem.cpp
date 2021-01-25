///////////////////////////////////////////////////////////////////////////////
// MyriadclockSettings class
// Class for storing app wide settings
///////////////////////////////////////////////////////////////////////////////

#include "MIOTConfigItem.h"
#include <stdio.h> 
#include <algorithm>
#include <time.h> // time_t, struct tm, time, mktime

//
// Constructor, create/fill this config item
MIOTConfigItem::MIOTConfigItem(const uint16_t id, const EConfigType type, const std::string name, const bool secure = true, const std::string synopsis = std::string())        
    : m_id(id)
    , m_eType(type)
    , m_sName(name)
    , m_sSynopsis(synopsis)
    , m_fSecure(secure)
{        
}

//
// Set a time_t value, get the date part
void MIOTConfigItem::setDateValue(const time_t newDate)
{
    if (m_eType == CT_DATE)
        m_value = (day(newDate) << 24) | (month(newDate) << 16) | year(newDate);
}

//
// Return the date value
time_t MIOTConfigItem::getDateValue()
{
    tmElements_t tmSet;
    tmSet.Year = 2000;
    tmSet.Month =  (m_value & 0x00FF0000) >> 16;
    tmSet.Day = (m_value & 0xFF000000) >> 24;
    return makeTime(tmSet);
}

//
// Returns the current value as a string
std::string MIOTConfigItem::getValueString() 
{
    if (m_eType == CT_WIFI ||
        m_eType == CT_STRING)
    {
        return m_valueString;
    }
    else
    {
        char stemp[32];
        snprintf(stemp, 32, "%08X", m_value);
        return std::string(stemp);
    }    
}


//
// Simply remove all options
//
void MIOTConfigItem::clearOptions()
{
    m_vecOptions.clear();
}

//
// Add a new option for this configuration item, only applies to 
// optionValue: The value/number/index of this option
// optionText: The text of this option that is displayed in a combobox/selectionbox
void MIOTConfigItem::addOption(const uint8_t optionValue, const std::string optionText)
{
    MIOTConfigOption_t option = { optionValue, optionText };
    m_vecOptions.push_back(option);
}

//
// Get a specific option identified by a value
// optionValue: The value of this option
// Returns the option
MIOTConfigOption_t* MIOTConfigItem::getOption(const uint8_t optionValue)
{
    for (int n = 0; n < m_vecOptions.size(); n++)
    {
        if (m_vecOptions[n].m_value == optionValue)
            return &(m_vecOptions[n]);
    }
    return NULL;
}

//
// Encode this config item into a byte array/buffer
// pdata: pointer to the buffer
// dataLen: length of the buffer
// Note that the buffer should (for now) hold at least 256 bytes
uint8_t MIOTConfigItem::encode(uint8_t *pdata, int dataLen)
{
    int slen = 0;
    if (dataLen < 256) return 0; // TODO CALCULATE THE DESIRED BUFFER/DATALENGTH

    // Small header with ID + Type
    pdata[0] = m_id;
    pdata[1] = (uint8_t) m_eType;

    // Add the flags
    pdata[2] = 0x00;
    if (m_fSecure) pdata[2] |= 0x01;

    // Start at byte 3
    int idx = 3;

    // Add the config item name    
    slen = (uint8_t) std::min((int) m_sName.length(), 0xF0);
    pdata[idx++] = slen;
    for(int n = 0; n < slen; n++)
        pdata[idx++] = (uint8_t) m_sName.c_str()[n];

    // Add the synopsis (=description)
    slen = (uint8_t) std::min((int) m_sSynopsis.length(), 0xF0);
    pdata[idx++] = slen;
    for(int n = 0; n < slen; n++)
        pdata[idx++] = m_sSynopsis.c_str()[n];

    // Add the data
    switch (m_eType)
    {
    case CT_STRING:
        slen = (uint8_t) std::min((int) m_valueString.length(), 0xF0);
        pdata[idx++] = slen;
        for(int i = 0; i < slen; i++)
            pdata[idx++] = m_valueString.c_str()[i];
        break;

    case CT_WIFI:
    case CT_OPTION:
        {
            // First byte is the actual data (no more then 256 options are allowed)
            pdata[idx++] = (uint8_t)(m_value & 0x000000FF);
            // Next byte is the number of options
            int numberOfOptionsIndex = idx; // Store this position (we need it later)
            pdata[idx++] = (uint8_t)(m_vecOptions.size());
            // Next are all the options, first the value followed by the text
            for(int n = 0; n < m_vecOptions.size(); n++)
            {
                int optionLen = 2 + m_vecOptions[n].m_sName.length();
                if (idx + optionLen > dataLen)
                {
                    // Doesn't fit, quit adding options
                    // But update the number of items we actualy added
                    pdata[numberOfOptionsIndex] = (uint8_t)(n);
                    break; 
                }

                // Add the option value (single byte)
                pdata[idx++] = m_vecOptions[n].m_value;
                slen = (uint8_t) std::min((int) m_vecOptions[n].m_sName.length(), 0xF0);
                // Add the name length  
                pdata[idx++] = slen; 
                for(int i = 0; i < slen; i++)
                    pdata[idx++] = m_vecOptions[n].m_sName.c_str()[i]; // And add the option name
            }
        }
        break;

    case CT_SLIDER:
        pdata[idx++] = (uint8_t)(m_value & 0x000000FF);
        break;
    
    case CT_RGBCOLOR:
    case CT_DATE:
    case CT_TIME:
    case CT_UINT32:
        pdata[idx++] = (uint8_t)((m_value & 0xFF000000) >> 24);
        pdata[idx++] = (uint8_t)((m_value & 0x00FF0000) >> 16);
        pdata[idx++] = (uint8_t)((m_value & 0x0000FF00) >> 8);
        pdata[idx++] = (uint8_t)((m_value & 0x000000FF) >> 0);
        break;

    default:
        break;
    }

    return idx;
}


//
// Decode an incoming config item
// data: string containing the data
bool MIOTConfigItem::decode(std::string data)//, uint8_t *pdata)
{
    switch (m_eType)
    {
    case CT_STRING:
        m_valueString = data; 
        break;

    case CT_WIFI:
        // The first byte contains the index of the WiFi option
        m_value = data[0];
        // Next bytes contain the passphrase
        if (data.length() > 1)
            m_valueString = data.substr(1); 
        break;

    case CT_SLIDER:
        m_value = data[0] % 100;
        break;

    case CT_OPTION:
        m_value = data[0];
        break;
    
    case CT_RGBCOLOR:
    case CT_DATE:
    case CT_TIME:
    case CT_UINT32:
        m_value = (uint32_t)(data[0] << 24) | (uint32_t)(data[1] << 16) | (uint32_t)(data[2] << 8) | (uint32_t)(data[3]);
        break;

    default:
        break;
    }
    return true;
}