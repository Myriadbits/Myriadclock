#pragma once

#include <string>
#include <vector>
#include "BLEConfigItemUInt8.h"

typedef struct 
{
    uint8_t         m_value; // The value of the config option
    std::string     m_sName; // Name of this option item
} MIOTConfigOption_t;


///////////////////////////////////////////////////////////////////////////////
// Single MIOT Configuration item
///////////////////////////////////////////////////////////////////////////////
class BLEConfigItemOption : public BLEConfigItemUInt8
{
public:
    BLEConfigItemOption(uint16_t id, EConfigType type, const std::string name, uint8_t defaultValue, bool secure = true, const std::string synopsis = std::string())
        : BLEConfigItemUInt8(id, type, name, defaultValue, secure, synopsis)
    {        
    }

    BLEConfigItemOption(uint16_t id, const std::string name, uint8_t defaultValue, bool secure = true, const std::string synopsis = std::string())
        : BLEConfigItemUInt8(id, EConfigType::CT_OPTION, name, defaultValue, secure, synopsis)
    {        
    }

    void clearOptions();
    void addOption(const uint8_t optionValue, const std::string optionText);
    MIOTConfigOption_t* getOption(const uint8_t optionValue);

protected:
    virtual int onEncodeData(uint8_t *pdata, int dataLen, int idx);

protected:
    std::vector<MIOTConfigOption_t> m_vecOptions;
};
