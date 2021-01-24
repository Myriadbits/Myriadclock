///////////////////////////////////////////////////////////////////////////////
// MyriadclockSettings class
// Class for storing app wide settings
///////////////////////////////////////////////////////////////////////////////

#include "MyriadclockSettings.h"

//
// Initialize, read all settings
//
void MyriadclockSettings::Initialize()
{
    m_preferences.begin("Myriadclock", false);
    Load();
}

//
// Store all settings to non-volatile storage
//
void MyriadclockSettings::Store()
{
    m_preferences.putString("ssid", sSSID.c_str());
    m_preferences.putString("passphrase", sPassPhrase.c_str());
    m_preferences.putUInt("colorTime", colTime);
    m_preferences.putUInt("colorWeekday", colWeekday);
    m_preferences.putUInt("colorDate", colDate);
    m_preferences.putUInt("colorBack", colBackground);
    m_preferences.putShort("brightnessDay", nBrightnessDay);
    m_preferences.putShort("brightnessNight", nBrightnessNight);
    m_preferences.putShort("brightnessBack", nBrightnessBackground);
    m_preferences.putShort("layout", nLayout);
    m_preferences.putInt("doTime", eDisplayOptionsTime);
    m_preferences.putInt("doWeekday", eDisplayOptionsWeekday);
    m_preferences.putInt("doDate", eDisplayOptionsDate);
    m_preferences.putInt("doBirthdays", eDisplayOptionsBirthday);
    m_preferences.putInt("doHolidays", eDisplayOptionsHoliday);

    char buff[32];
    for(int n = 0; n < MAX_BIRTHDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "birthdays[%d]", n);
        m_preferences.putULong64(buff, dateBirthdays[n]);
    }
    for(int n = 0; n < MAX_HOLIDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "holidays[%d]", n);
        m_preferences.putULong64(buff, dateHolidays[n]);
    }
}

//
// Load all settings from non-volatile storage
//
void MyriadclockSettings::Load()
{
    sSSID = std::string(m_preferences.getString("ssid", "").c_str());
    sPassPhrase = std::string(m_preferences.getString("passphrase", "").c_str());            

    colTime = m_preferences.getUInt("colorTime", 0x00FF00);
    colWeekday = m_preferences.getUInt("colorWeekday", 0xFFA500);
    colDate = m_preferences.getUInt("colorDate", 0xE59400);
    colBackground = 0xFFFFFF; // m_preferences.getUInt("colorBack", 0xFFFFFF);

    nBrightnessDay = m_preferences.getShort("brightnessDay", 80);
    nBrightnessNight = m_preferences.getShort("brightnessNight", 30);
    nBrightnessBackground = m_preferences.getShort("brightnessBack", 4);

    eDisplayOptionsTime = (EDisplayOptions) m_preferences.getInt("doTime", DO_NORMAL);
    eDisplayOptionsWeekday = (EDisplayOptions) m_preferences.getInt("doWeekday", DO_NORMAL);
    eDisplayOptionsDate = (EDisplayOptions) m_preferences.getInt("doDate", DO_NORMAL);
    eDisplayOptionsBirthday = (EDisplayOptions) m_preferences.getInt("doBirthdays", DO_COLOR_PARTY_MINUTE);
    eDisplayOptionsHoliday = (EDisplayOptions) m_preferences.getInt("doHolidays", DO_NORMAL);
    nLayout = m_preferences.getShort("layout", 0);

    char buff[32];
    for(int n = 0; n < MAX_BIRTHDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "birthdays[%d]", n);
        if (n == 0) dateBirthdays[n] = m_preferences.getULong64(buff, 62352000L); // 23-12-1971
        else dateBirthdays[n] = m_preferences.getULong64(buff, 0L);
    }
    for(int n = 0; n < MAX_HOLIDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "holidays[%d]", n);
        if (n == 0) dateHolidays[n] = m_preferences.getULong64(buff, 946728000L); // New years day
        else if (n == 1) dateHolidays[n] = m_preferences.getULong64(buff, 977745600L); // Christmas 1
        else if (n == 2) dateHolidays[n] = m_preferences.getULong64(buff, 977832000L); // Christmas 2
        else dateHolidays[n] = m_preferences.getULong64(buff, 0L);
    }
}

//
// Register all settings with the MIOT library
void MyriadclockSettings::registerConfigItems(MIOTConfigurator *pmiot)
{
    pmiot->addConfigItem(1, CT_WIFI, "WiFi SSID", true);
    
    pmiot->addConfigItem(10, CT_RGBCOLOR, "Time color", true, "Color of the hours/minutes part");
    pmiot->addConfigItem(11, CT_RGBCOLOR, "Weekday Color", true, "Color of the day of the week");
    pmiot->addConfigItem(12, CT_RGBCOLOR, "Date color", true, "Color of the date part");

    pmiot->addConfigItem(13, CT_SLIDER, "Brightness Day", false, "Brightness during the day");
    pmiot->addConfigItem(14, CT_SLIDER, "Brightness Night", false, "Brightness during the night");
    pmiot->addConfigItem(15, CT_SLIDER, "Background Brightness", false, "Brightness of the background");

    MIOTConfigItem* pconfig = pmiot->addConfigItem(20, CT_OPTION, "Time options");
    pconfig->addOption((uint8_t) DO_NORMAL, "Normal");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLENORMAL, "Color cycle normal");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLEHOUR, "Color cycle hourly");
    pconfig->addOption((uint8_t) DO_COLOR_PARTY_SLOW, "Party colors slow");
    pconfig->addOption((uint8_t) DO_COLOR_PARTY_QUICK, "Party colors quick");
    pconfig->addOption((uint8_t) DO_COLOR_PARTY_MINUTE, "Party colors minute");
}

//
// Callback that is called when a config item is read 
void MyriadclockSettings::configItemRead(MIOTConfigItem *pconfigItem)
{
    switch (pconfigItem->getId())
    {
    case 1: pconfigItem->setValue(sSSID); break;
    case 2: pconfigItem->setValue(""); break; // ALWAYS send back an empty passphrase!

    case 10: pconfigItem->setValue(colTime); break;
    case 11: pconfigItem->setValue(colWeekday); break;
    case 12: pconfigItem->setValue(colDate); break;
    
    case 13: pconfigItem->setValue(nBrightnessDay); break;
    case 14: pconfigItem->setValue(nBrightnessNight); break;
    case 15: pconfigItem->setValue(nBrightnessBackground); break;

    case 20: pconfigItem->setValue(eDisplayOptionsTime); break;

    default:
        break;
    }
}

//
// Callback that is called when a config item changes 
void MyriadclockSettings::configItemWrite(MIOTConfigItem *pconfigItem)
{
    switch (pconfigItem->getId())
    {
        case 1: sSSID = pconfigItem->getValueString(); break;
        case 2: sPassPhrase = pconfigItem->getValueString(); break;

        case 10: colTime = pconfigItem->getValue(); break;
        case 11: colWeekday = pconfigItem->getValue(); break;
        case 12: colDate = pconfigItem->getValue(); break;

        case 13: nBrightnessDay = (pconfigItem->getValue() % 100); break;
        case 14: nBrightnessNight = (pconfigItem->getValue() % 100); break;
        case 15: nBrightnessBackground = (pconfigItem->getValue() % 100); break;

        case 20: eDisplayOptionsTime = (EDisplayOptions) pconfigItem->getValue(); break;

        default:
            // Nothing set, return
            return;
    }

    // Something probably changed, safe the new settings
    Store();
}
