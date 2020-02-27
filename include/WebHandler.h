#pragma once

#include "Esp.h"
#include "FastLED.h"            // Fastled library to control the LEDs
#include "ClockLayout.h"        // Clock layout, needed for ledpos_t
#include <Timezone.h>
#include <WebServer.h>
#include <string>
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
    String      GetHTMLPage();

    bool        ParseHexNumber(uint32_t &setting, String key, String hexNum);
    bool        ParseDisplayOptions(MyriadclockSettings::displayOptions &setting, String key, String value);

    String      GetHexString(uint32_t value);
    String      GetDisplayOptionsString(MyriadclockSettings::displayOptions &value);

private:
    WebServer  &mServer;
    MyriadclockSettings &mSettings;
};

