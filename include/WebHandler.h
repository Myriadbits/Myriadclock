#pragma once

#include <WebServer.h>
#include "MyriadclockSettings.h"

#define MAXCOMMANDLEN               16
#define MAXCOMMANDDESCRIPTIONLEN    80

///////////////////////////////////////////////////////////////////////////////
// Handler to parse web API calls
///////////////////////////////////////////////////////////////////////////////
class WebHandler
{
public:
    WebHandler(WebServer &server, MyriadclockSettings &settings);
    bool        ProcessWebCall();

private:
    void        RootPage(void);
    String*     GetHTMLPage();

    bool        ParseHexNumber(uint32_t &setting, String key, String hexNum);
    bool        ParseDisplayOptions(MyriadclockSettings::EDisplayOptions &setting, String key, String value);
    bool        ParseNumber(int16_t &setting, String key, String value);

    String      GetHexString(uint32_t value);
    String      GetDisplayOptionsString(MyriadclockSettings::EDisplayOptions &value);

private:
    WebServer   &mServer;
    MyriadclockSettings &mSettings;
};

