///////////////////////////////////////////////////////////////////////////////
// MyriadclockSettings class
// Class for storing app wide settings
///////////////////////////////////////////////////////////////////////////////

#include "MIOTConfigItem.h"


MIOTConfigItem::MIOTConfigItem(uint16_t id, EConfigType type, std::string name, std::string synopsis, std::string unit)        
        : m_id(id)
        , m_eType(type)
        , m_sName(name)
        , m_sSynopsis(synopsis)
        , m_sUnit(unit)
{        
}

std::string MIOTConfigItem::getValueString() 
{
    char stemp[32];
    if (m_eType == CT_WIFI_SSID ||
        m_eType == CT_WIFI_PASSPHRASE ||
        m_eType == CT_STRING)
    {
        return m_valueString;
    }
    else if (m_eType == CT_RGBCOLOR)
    {
        snprintf(stemp, 32, "#%02X%02X%02X", (uint8_t)((m_value & 0x00FF0000) >> 16), (uint8_t)((m_value & 0x0000FF00) >> 8), (uint8_t)(m_value & 0x000000FF));
    }
    else
    {
        snprintf(stemp, 32, "%08X", m_value);
    }    
    return std::string(stemp);    
}


uint8_t MIOTConfigItem::encode(uint8_t *pdata, int dataLen)
{
    int slen = 0;
    if (dataLen < 256) return 0; // TODO CALCULATE THE DESIRED BUFFER/DATALENGTH

    pdata[0] = m_id;
    pdata[1] = (uint8_t) m_eType;

    int idx = 2;
    slen = (uint8_t) min((int) m_sName.length(), 0xF0);
    pdata[idx++] = slen;
    for(int n = 0; n < slen; n++)
        pdata[idx++] = (uint8_t) m_sName.c_str()[n];

    slen = (uint8_t) min((int) m_sSynopsis.length(), 0xF0);
    pdata[idx++] = slen;
    for(int n = 0; n < slen; n++)
        pdata[idx++] = m_sSynopsis.c_str()[n];

    slen = (uint8_t) min((int) m_sUnit.length(), 0xF0);
    pdata[idx++] = slen;
    for(int n = 0; n < slen; n++)
        pdata[idx++] = m_sUnit.c_str()[n];

    switch (m_eType)
    {
    case CT_WIFI_SSID:
    case CT_WIFI_PASSPHRASE:
    case CT_RGBCOLOR:
    case CT_STRING:
        slen = (uint8_t) min((int) m_valueString.length(), 0xF0);
        pdata[idx++] = slen;
        for(int n = 0; n < slen; n++)
            pdata[idx++] = m_valueString.c_str()[n];
        break;

    case CT_SLIDER:
        pdata[idx++] = (uint8_t)(m_value & 0x000000FF);
        break;
            
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


bool MIOTConfigItem::decode(std::string data, uint8_t *pdata)
{
    switch (m_eType)
    {
    case CT_WIFI_SSID:
    case CT_WIFI_PASSPHRASE:
    case CT_STRING:
        m_valueString = data; 
        break;

    case CT_SLIDER:
        {
            m_value = pdata[1] % 100;
        }
        break;
    
    case CT_RGBCOLOR:
        {
            int r, g, b;
            sscanf(data.c_str(), "#%02x%02x%02x", &r, &g, &b);
            m_value = (uint32_t)(0 << 24) | (uint32_t)(r << 16) | (uint32_t)(g << 8) | (uint32_t)(b);
        }
        break;

    case CT_UINT32:
        m_value = (uint32_t)(pdata[1] << 24) | (uint32_t)(pdata[2] << 16) | (uint32_t)(pdata[3] << 8) | (uint32_t)(pdata[4]);
        break;

    default:
        break;
    }
    return true;
}