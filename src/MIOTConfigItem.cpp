///////////////////////////////////////////////////////////////////////////////
// MyriadclockSettings class
// Class for storing app wide settings
///////////////////////////////////////////////////////////////////////////////

#include "MIOTConfigItem.h"

MIOTConfigItem::MIOTConfigItem(uint8_t id, EConfigType type, std::string name, std::string synopsis, std::string unit)        
        : m_id(id)
        , m_eType(type)
        , m_sName(name)
        , m_sSynopsis(synopsis)
        , m_sUnit(unit)
{        
}


uint8_t* MIOTConfigItem::encode(uint8_t* outputLen)
{
    int slen = 0;
    uint8_t* pdata = new uint8_t[256]; // 250 datalength + header

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

    pdata[idx++] = (m_value & 0xFF000000) >> 24;
    pdata[idx++] = (m_value & 0x00FF0000) >> 16;
    pdata[idx++] = (m_value & 0x0000FF00) >> 8;
    pdata[idx++] = (m_value & 0x000000FF) >> 0;

    if (outputLen != NULL)
        *outputLen = idx;

    return pdata;
}
