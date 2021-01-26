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
    m_preferences.putInt("doSpecial", eDisplayOptionsSpecial);

    char buff[32];
    for(int n = 0; n < MAX_BIRTHDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "birthdays[%d]", n);
        m_preferences.putUInt(buff, dateBirthdays[n]);
    }
    for(int n = 0; n < MAX_HOLIDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "holidays[%d]", n);
        m_preferences.putUInt(buff, dateHolidays[n]);
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

    eDisplayOptionsTime = (EDisplayOptions) m_preferences.getInt("doTime", DO_STATIC);
    eDisplayOptionsWeekday = (EDisplayOptions) m_preferences.getInt("doWeekday", DO_STATIC);
    eDisplayOptionsDate = (EDisplayOptions) m_preferences.getInt("doDate", DO_STATIC);

    eDisplayOptionsSpecial = (EDisplayOptions) m_preferences.getInt("doSpecial", DO_COLOR_PARTY_MINUTE);
    nLayout = m_preferences.getShort("layout", 0);

    char buff[32];
    for(int n = 0; n < MAX_BIRTHDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "birthdays[%d]", n);
        if (n == 0) dateBirthdays[n] = m_preferences.getUInt(buff,  0x170C07B3); // 23-12-1971
        else dateBirthdays[n] = m_preferences.getUInt(buff, 0L);
    }
    for(int n = 0; n < MAX_HOLIDAYS; n++)
    {
        snprintf(buff, sizeof(buff), "holidays[%d]", n);
        if (n == 0) dateHolidays[n] = m_preferences.getUInt(buff, 0x010107E5); // New years day
        else if (n == 1) dateHolidays[n] = m_preferences.getUInt(buff, 0x190C07E5); // Christmas 1
        else if (n == 2) dateHolidays[n] = m_preferences.getUInt(buff, 0x1A0C07E5); // Christmas 2
        else dateHolidays[n] = m_preferences.getUInt(buff, 0L);
    }
}

//
// Register all settings with the MIOT library
void MyriadclockSettings::registerConfigItems(MIOTConfigurator *pmiot)
{
    MIOTConfigItem* pconfig;

    pmiot->addConfigItem(1, CT_WIFI, "WiFi SSID", true);

    pconfig = pmiot->addConfigItem(3, CT_OPTION, "Clock layout", true);
    pconfig->addOption((uint8_t) 0, "Dutch V2");
    pconfig->addOption((uint8_t) 1, "Dutch V1");
    pconfig->addOption((uint8_t) 2, "English");   

    pmiot->addConfigItem(10, CT_RGBCOLOR, "Time color", true, "Color of the hours/minutes part");
    pmiot->addConfigItem(11, CT_RGBCOLOR, "Weekday Color", true, "Color of the day of the week");
    pmiot->addConfigItem(12, CT_RGBCOLOR, "Date color", true, "Color of the date part");

    pmiot->addConfigItem(13, CT_SLIDER, "Brightness Day", false, "Brightness during the day");
    pmiot->addConfigItem(14, CT_SLIDER, "Brightness Night", false, "Brightness during the night");
    pmiot->addConfigItem(15, CT_SLIDER, "Background Brightness", false, "Brightness of the background");

    pconfig = pmiot->addConfigItem(20, CT_OPTION, "Time color options");
    pconfig->addOption((uint8_t) DO_STATIC, "Manual");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLENORMAL, "Color cycle");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLEHOUR, "Color cycle hourly");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_AYURVEDA, "Ayurveda colors daily");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_THAI, "Thai colors daily");

    pconfig = pmiot->addConfigItem(21, CT_OPTION, "Weekday color options");
    pconfig->addOption((uint8_t) DO_STATIC, "Manual");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLENORMAL, "Color cycle");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLEHOUR, "Color cycle hourly");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_AYURVEDA, "Ayurveda colors daily");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_THAI, "Thai colors daily");

    pconfig = pmiot->addConfigItem(22, CT_OPTION, "Date color options");
    pconfig->addOption((uint8_t) DO_STATIC, "Manual");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLENORMAL, "Color cycle");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLEHOUR, "Color cycle hourly");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_AYURVEDA, "Ayurveda colors daily");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_THAI, "Thai colors daily");

    pconfig = pmiot->addConfigItem(23, CT_OPTION, "Special day options");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLENORMAL, "Color cycle");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLEHOUR, "Color cycle hourly");
    pconfig->addOption((uint8_t) DO_COLOR_PARTY_SLOW, "Party colors slow");
    pconfig->addOption((uint8_t) DO_COLOR_PARTY_QUICK, "Party colors quick");
    pconfig->addOption((uint8_t) DO_COLOR_PARTY_MINUTE, "Party colors minute");

    pmiot->addConfigItem(30, CT_DATE, "Birthday 1", false);
    pmiot->addConfigItem(31, CT_DATE, "Birthday 2", false);
    pmiot->addConfigItem(32, CT_DATE, "Birthday 3", false);
    pmiot->addConfigItem(33, CT_DATE, "Birthday 4", false);
}

//
// Callback that is called when a config item is read 
void MyriadclockSettings::configItemRead(MIOTConfigItem *pconfigItem)
{
    switch (pconfigItem->getId())
    {
    case 1: pconfigItem->setValue(sSSID); break;
    case 2: pconfigItem->setValue(""); break; // ALWAYS send back an empty passphrase!

    case 3: pconfigItem->setValue(nLayout); break;

    case 10: pconfigItem->setValue(colTime); break;
    case 11: pconfigItem->setValue(colWeekday); break;
    case 12: pconfigItem->setValue(colDate); break;
    
    case 13: pconfigItem->setValue(nBrightnessDay); break;
    case 14: pconfigItem->setValue(nBrightnessNight); break;
    case 15: pconfigItem->setValue(nBrightnessBackground); break;

    case 20: pconfigItem->setValue(eDisplayOptionsTime); break;
    case 21: pconfigItem->setValue(eDisplayOptionsWeekday); break;
    case 22: pconfigItem->setValue(eDisplayOptionsDate); break;
    case 23: pconfigItem->setValue(eDisplayOptionsSpecial); break; 

    case 30: pconfigItem->setValue(dateBirthdays[0]); break; // Date value is 0xddmmyyyy
    case 31: pconfigItem->setValue(dateBirthdays[1]); break;
    case 32: pconfigItem->setValue(dateBirthdays[2]); break;
    case 33: pconfigItem->setValue(dateBirthdays[3]); break;

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

        case 3: nLayout = pconfigItem->getValue(); break;

        case 10: colTime = pconfigItem->getValue(); break;
        case 11: colWeekday = pconfigItem->getValue(); break;
        case 12: colDate = pconfigItem->getValue(); break;

        case 13: nBrightnessDay = (pconfigItem->getValue() % 100); break;
        case 14: nBrightnessNight = (pconfigItem->getValue() % 100); break;
        case 15: nBrightnessBackground = (pconfigItem->getValue() % 100) / 5; break; // Limit background brightness to 0 - 20

        case 20: eDisplayOptionsTime = (EDisplayOptions) pconfigItem->getValue(); break;
        case 21: eDisplayOptionsWeekday = (EDisplayOptions) pconfigItem->getValue(); break;
        case 22: eDisplayOptionsDate = (EDisplayOptions) pconfigItem->getValue(); break;
        case 23: eDisplayOptionsSpecial = (EDisplayOptions) pconfigItem->getValue(); break;

        case 30: dateBirthdays[0] = pconfigItem->getValue(); break;
        case 31: dateBirthdays[1] = pconfigItem->getValue(); break;
        case 32: dateBirthdays[2] = pconfigItem->getValue(); break;
        case 33: dateBirthdays[3] = pconfigItem->getValue(); break;

        default:
            // Nothing set, return
            return;
    }

    // Something probably changed, safe the new settings
    Store();
}
