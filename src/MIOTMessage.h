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
    MIOTMessage(uint8_t command, uint8_t number)
        : m_protocolVersion(MIOTMESSAGE_PROTOCOL_VERSION)
        , m_command(command)
        , m_number(number)
    {        
    }

    MIOTMessage(std::string value)
    {        
        decode(value);
    }
    
    uint8_t getProtocolVersion() { return m_protocolVersion; }
    uint8_t getCommand() { return m_command; }
    uint8_t getNumber() { return m_number; }

    std::string encode()
    {
        m_data[0] = m_command;
        size_t len = 0;
        unsigned char* encoded = base64_encode(m_data, 1, &len); 
        return std::string((const char*) encoded);
    }

    void decode(std::string value)
    {
        size_t len = 0;
        unsigned char* pdata = base64_decode((const unsigned char *) value.c_str(), value.length(), &len); 
        memcpy(m_data, pdata, len);
        if (len > 0)
        {
            m_protocolVersion = m_data[0];
            if (m_protocolVersion == MIOTMESSAGE_PROTOCOL_VERSION)
            {
                m_command = m_data[1];
                m_number = m_data[2];
            }
        }
    }

private:
    uint8_t         m_protocolVersion;
    uint8_t         m_command;
    uint8_t         m_number;
    uint8_t         m_data[32];
};
