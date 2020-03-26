///////////////////////////////////////////////////////////////////////////////
// WebHandler class
// Base class for controlling the display/led states
///////////////////////////////////////////////////////////////////////////////

#include "WebHandler.h"
#include <string>

//
// Handle the webpage
//
void WebHandler::RootPage(void)
{
    // Just forward the call to the webserver handler
    ProcessWebCall();
}

WebHandler::WebHandler(WebServer &server, MyriadclockSettings &settings)
    : mServer{server}, mSettings{settings} 
{
    mServer.on("/", [this](){ this->RootPage();});
}

//
// Return the entire webpage
//
String* WebHandler::GetHTMLPage()
{
    // Include the entire html file at once by using the preprocessor
    static const char sIndexPage[] =    
        #include "..\www\index.html"
    ;    
    return new String(sIndexPage);
}

//
// Parse a hexadecimal number (used for parsing colors), return the uint32 value
//
bool WebHandler::ParseHexNumber(uint32_t &setting, String key, String hexNum)
{
    if (mServer.hasArg(key))
    {
        String hex = hexNum;
        if (hexNum.startsWith("#"))
            hex = hexNum.substring(1);
        uint32_t value = std::strtoul(hex.c_str(), 0, 16);

        if (setting != value)
        {
            Serial.printf("- Value '%s' is changed to %06X ('%s')\n", key.c_str(), value, hexNum.c_str());
            setting = value;
            return true;
        }
    }
    return false;
}

//
// Parse a number/option
//
bool WebHandler::ParseDisplayOptions(MyriadclockSettings::EDisplayOptions &setting, String key, String valueText)
{
    if (mServer.hasArg(key))
    {
        uint32_t value = std::strtoul(valueText.c_str(), 0, 10);
        if (setting != value)
        {
            Serial.printf("- Value '%s' is changed to %d\n", key.c_str(), value);
            setting = (MyriadclockSettings::EDisplayOptions) value;
            return true;
        }
    }
    return false;
}

//
// Parse a normal number, return true if the value has changed
//
bool WebHandler::ParseNumber(int16_t &setting, String key, String val, long minValue, long maxValue)
{
    if (mServer.hasArg(key))
    {
        int16_t value = std::strtol(val.c_str(), 0, 10);
        if (value < minValue) value = minValue;
        if (value > maxValue) value = maxValue;
        if (setting != value)
        {
            Serial.printf("- Value '%s' is changed to %06X ('%s')\n", key.c_str(), value, val.c_str());
            setting = value;
            return true;
        }
    }
    return false;
}

//
// Return the hex string of a specific value
//
String WebHandler::GetHexString(uint32_t value)
{
    char buff[32];
    snprintf(buff, sizeof(buff), "#%06X", value);
    return String(buff);
}

//
// Return the numer string of a specific value
//
String WebHandler::GetNumberString(uint32_t value)
{
    char buff[32];
    snprintf(buff, sizeof(buff), "%d", value);
    return String(buff);
}

//
// Return the display options string of a specific value
//
String WebHandler::GetDisplayOptionsString(MyriadclockSettings::EDisplayOptions &value)
{
    char buff[32];
    snprintf(buff, sizeof(buff), "%d", (int) value);
    return String(buff);
}


//
// Process the web call from a webserver
//
bool WebHandler::ProcessWebCall()
{
    // Parse any changed settings
    bool changed = false;
    changed |= ParseHexNumber(mSettings.colTime, "colTime", mServer.arg("colTime"));        
    changed |= ParseHexNumber(mSettings.colWeekday, "colWeekday", mServer.arg("colWeekday"));
    changed |= ParseHexNumber(mSettings.colDate, "colDate", mServer.arg("colDate"));
    changed |= ParseHexNumber(mSettings.colBackground, "colBackground", mServer.arg("colBackground"));
    changed |= ParseDisplayOptions(mSettings.eDisplayOptionsTime, "displayOptionTime", mServer.arg("displayOptionTime"));
    changed |= ParseDisplayOptions(mSettings.eDisplayOptionsWeekday, "displayOptionWeekday", mServer.arg("displayOptionWeekday"));
    changed |= ParseDisplayOptions(mSettings.eDisplayOptionsDate, "displayOptionDate", mServer.arg("displayOptionDate"));
    changed |= ParseNumber(mSettings.nBrightnessDay, "brightnessDay", mServer.arg("brightnessDay"), 1, 100);
    changed |= ParseNumber(mSettings.nBrightnessNight, "brightnessNight", mServer.arg("brightnessNight"), 1, 100);
    changed |= ParseNumber(mSettings.nBrightnessBackground, "brightnessBackground", mServer.arg("brightnessBackground"), 0, 100);

    // Whenever a setting has changed, store all settings
    if (changed)
    {
        Serial.printf("- Storing changes\n");
        mSettings.Store();
    }

    // Nothing in the post message, just return the entire page
    String* pageText = GetHTMLPage();
    
    // TODO: parse 
    pageText->replace("<!--sClockName-->", mSettings.sClockName.c_str());
    pageText->replace("<!--nSerialNumber-->", GetNumberString(mSettings.nSerialNumber));
    pageText->replace("<!--colTime-->", GetHexString(mSettings.colTime));
    pageText->replace("<!--colWeekday-->", GetHexString(mSettings.colWeekday));
    pageText->replace("<!--colDate-->", GetHexString(mSettings.colDate));
    pageText->replace("<!--colBackground-->", GetHexString(mSettings.colBackground));
    pageText->replace("<!--displayOptionsTime-->", GetDisplayOptionsString(mSettings.eDisplayOptionsTime));
    pageText->replace("<!--displayOptionsWeekday-->", GetDisplayOptionsString(mSettings.eDisplayOptionsWeekday));
    pageText->replace("<!--displayOptionsDate-->", GetDisplayOptionsString(mSettings.eDisplayOptionsDate));
    pageText->replace("<!--brightnessDay-->", GetNumberString(mSettings.nBrightnessDay));
    pageText->replace("<!--brightnessNight-->", GetNumberString(mSettings.nBrightnessNight));
    pageText->replace("<!--brightnessBackground-->", GetNumberString(mSettings.nBrightnessBackground));

    mServer.send_P(200, "text/html", pageText->c_str());

    delete pageText;

    return true;
}
