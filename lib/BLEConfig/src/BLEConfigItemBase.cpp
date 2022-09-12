///////////////////////////////////////////////////////////////////////////////
// MyriadclockSettings class
// Class for storing app wide settings
///////////////////////////////////////////////////////////////////////////////

#include "../include/BLEConfigItemBase.h"
#include <stdio.h> 
#include <algorithm>
#include <time.h> // time_t, struct tm, time, mktime

//
// Constructor, create/fill this config item
BLEConfigItemBase::BLEConfigItemBase(uint16_t id, const EConfigType type, const std::string name, bool secure, const std::string synopsis)        
    : m_id(id)
    , m_eType(type)
    , m_sName(name)
    , m_sSynopsis(synopsis)
    , m_fSecure(secure)
{        
}

//
// Encode this config item into a byte array/buffer
// pdata: pointer to the buffer
// dataLen: length of the buffer
// Note that the buffer should (for now) hold at least 256 bytes
int BLEConfigItemBase::encode(uint8_t *pdata, int dataLen)
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
    return onEncodeData(pdata, dataLen, idx);
}


//
// Decode an incoming config item
// data: string containing the data
bool BLEConfigItemBase::decode(std::string data)
{
    onDecodeData(data);
    return true;
}

//
// Load this config item from the preferences
void BLEConfigItemBase::load(Preferences &preferences)
{
    // Create the preference key
    char sprefKey[16];
    snprintf(sprefKey, 16, "key_%d", m_id);
    onLoad(preferences, sprefKey);    
}


//
// Store this config item to the preferences
void BLEConfigItemBase::store(Preferences &preferences)
{
    // Create the preference key
    char sprefKey[16];
    snprintf(sprefKey, 16, "key_%d", m_id);
    onStore(preferences, sprefKey);
}


