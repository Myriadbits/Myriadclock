///////////////////////////////////////////////////////////////////////////////
// MyriadclockSettings class
// Class for storing app wide settings
///////////////////////////////////////////////////////////////////////////////

#include "MIOTMessage.h"

extern "C" {
#include "crypto/base64.h"
}

MIOTMessage::MIOTMessage(uint8_t command, uint8_t number, uint8_t datalength, uint8_t *pdata)
{        
    m_data[0] = MIOTMESSAGE_PROTOCOL_VERSION;
    m_data[1] = command;
    m_data[2] = number;
    m_data[3] = (uint8_t) min((int)datalength, 0xF0);
    memcpy(&(m_data[4]), pdata, m_data[3]);
}

MIOTMessage::MIOTMessage(std::string value)
{        
    decode(value);
}


std::string MIOTMessage::encode()
{
    unsigned char* encoded = base64_encode(m_data, 1, NULL); 
    std::string output((const char*) encoded);
    free(encoded);
    return output;
}

void MIOTMessage::decode(std::string value)
{
    size_t len = 0;
    unsigned char* pdata = base64_decode((const unsigned char *) value.c_str(), value.length(), &len); 
    memcpy(m_data, pdata, len);
    free(pdata);
}