#pragma once

#include "FastLED.h"
#include <Preferences.h>

extern "C" {
#include "crypto/base64.h"
}

using namespace std;

#define MIOTMESSAGE_PROTOCOL_VERSION        1


///////////////////////////////////////////////////////////////////////////////
// MIOT BLE Message
///////////////////////////////////////////////////////////////////////////////
class MIOTMessage
{
public:
    MIOTMessage(uint8_t command, uint8_t number, uint8_t datalength = 0, uint8_t *pdata = NULL)
    {        
        m_data[0] = MIOTMESSAGE_PROTOCOL_VERSION;
        m_data[1] = command;
        m_data[2] = number;
        m_data[3] = (uint8_t) min((int)datalength, 0xF0);
        memcpy(&(m_data[4]), pdata, m_data[3]);
    }

    MIOTMessage(std::string value)
    {        
        decode(value);
    }
    
    uint8_t getProtocolVersion() { return m_data[0]; }
    uint8_t getCommand() { return m_data[1]; }
    uint8_t getNumber() { return m_data[2]; }
    uint8_t getDataLength() { return m_data[3]; }

    std::string encode()
    {
        unsigned char* encoded = base64_encode(m_data, 1, NULL); 
        std::string output((const char*) encoded);
        free(encoded);
        return output;
    }

    void decode(std::string value)
    {
        size_t len = 0;
        unsigned char* pdata = base64_decode((const unsigned char *) value.c_str(), value.length(), &len); 
        memcpy(m_data, pdata, len);
        free(pdata);
    }

private:
    uint8_t         m_data[256]; // 250 datalength + header
};
