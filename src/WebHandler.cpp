///////////////////////////////////////////////////////////////////////////////
// WebHandler class
// Base class for controlling the display/led states
///////////////////////////////////////////////////////////////////////////////

#include "WebHandler.h"

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
String WebHandler::GetHTMLPage()
{
    // Include the entire html file at once by using the preprocessor
    const char *s =    
        #include "..\www\index.html"
    ;    
    return String(s);
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
bool WebHandler::ParseDisplayOptions(MyriadclockSettings::displayOptions &setting, String key, String valueText)
{
    if (mServer.hasArg(key))
    {
        uint32_t value = std::strtoul(valueText.c_str(), 0, 10);
        if (setting != value)
        {
            Serial.printf("- Value '%s' is changed to %d\n", key.c_str(), value);
            setting = (MyriadclockSettings::displayOptions) value;
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
// Return the display options string of a specific value
//
String WebHandler::GetDisplayOptionsString(MyriadclockSettings::displayOptions &value)
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
    changed |= ParseHexNumber(mSettings.colDate, "colDate", mServer.arg("colDate"));
    changed |= ParseDisplayOptions(mSettings.eDisplayOptionsTime, "displayOptionTime", mServer.arg("displayOptionTime"));
    changed |= ParseDisplayOptions(mSettings.eDisplayOptionsDate, "displayOptionDate", mServer.arg("displayOptionDate"));

    // Whenever a setting has changed, store all settings
    if (changed)
    {
        Serial.printf("- Storing changes");
        mSettings.Store();
    }

    // Nothing in the post message, just return the entire page
    String pageText = GetHTMLPage();
    pageText.replace("t#000002", GetHexString(mSettings.colTime));
    pageText.replace("d#000002", GetHexString(mSettings.colDate));
    pageText.replace("t#000001", GetDisplayOptionsString(mSettings.eDisplayOptionsTime));
    pageText.replace("d#000001", GetDisplayOptionsString(mSettings.eDisplayOptionsDate));

    mServer.send(200, "text/html", pageText);

    return true;
}
