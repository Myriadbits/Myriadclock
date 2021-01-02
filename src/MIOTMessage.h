#pragma once

#include "FastLED.h"
#include <Preferences.h>

using namespace std;

#define MIOTMESSAGE_PROTOCOL_VERSION        1


///////////////////////////////////////////////////////////////////////////////
// MIOT BLE Message
///////////////////////////////////////////////////////////////////////////////
class MIOTMessage
{
public:
    MIOTMessage(uint8_t command, uint8_t number, uint8_t datalength = 0, uint8_t *pdata = NULL);
    MIOTMessage(std::string value);
   
    uint8_t getProtocolVersion() { return m_data[0]; }
    uint8_t getCommand() { return m_data[1]; }
    uint8_t getNumber() { return m_data[2]; }
    uint8_t getDataLength() { return m_data[3]; }

    std::string encode();
    void decode(std::string value);

private:
    uint8_t         m_data[256]; // 250 datalength + header
};
