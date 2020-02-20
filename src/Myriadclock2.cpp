/*  Myriadclock
    Using WS2812 LED Panel

    Created by Jochem Bakker

    This code is in the public domain...

    You can: copy it, use it, modify it, share it or just plain ignore it!
    Thx!

    NTPClient: https://github.com/arduino-libraries/NTPClient
    Autoconnect stuff: https://github.com/Hieromon/AutoConnect
    TimeZone: https://github.com/JChristensen/Timezone
    Esp32FOTA: https://github.com/chrisjoyce911/esp32FOTA

    TODO:
    HOMEKIT: https://github.com/maximkulkin/esp-homekit

    s00500/ESPUI??
    Bootstrap: https://diyprojects.io/bootstrap-create-beautiful-web-interface-projects-esp8266/#.XkhzFWhKiUk
    
*/

#include <NTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <AutoConnect.h>
#include <time.h>
#include <Timezone.h>
#include <ESPmDNS.h>

#include "Console.h"
#include "ClockLayout.h"
#include "FastLED.h"       // Fastled library to control the LEDs

#include "DisplayStateBooting.h"
#include "DisplayStateWords.h"
#include "DisplayStateClock.h"
#include "DisplayStateUpdating.h"
#include "MyriadclockConfig.h"

using namespace std;

// Defines
#define DATA_PIN                13      // Connected to the data pin of the first LED strip
#define MAXSTATES               10      // Maximum number of states

CRGB colGreen = CRGB(0, 255, 0);

static CRGB     g_Leds[NUM_LEDS]; // Define the array of leds
static Console* g_pConsole = NULL;
static uint32_t g_timestamp  = 0;
static DisplayStateBase* g_arStates[MAXSTATES] = {0};
static int      g_nStateCounter = 0;
static int      g_nCurrentState = 0;
static bool     g_fNTPStarted = false;
static Preferences g_Preferences;
static int      g_nPreviousHour = 0;

WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and no offset
NTPClient timeClient(ntpUDP);

// Webservers
WebServer           g_server;
AutoConnect         g_acPortal(g_server);
AutoConnectUpdate   g_acUpdate("www.myriadbits.com", 80);  // Step #3


// Central European Time (Frankfurt, Paris)
TimeChangeRule      CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule      CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone            g_CE(CEST, CET);

// Get CHIP ID:
//chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
//	Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
//	Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.


//
// Time elapsed??
//
uint32_t Elapsed(uint32_t ts)
{
    uint32_t now = millis();
    if (ts < now) return (now - ts);
    return(now + (0xffffffff - ts));
}

//
// Activate another state
//
static void cmdStateChange(const char* command, int argc, char *argv[])
{
    // Find the command in the state array
    for(int n = 0; n < g_nStateCounter; n++)
    {
        if (strcmp(command, g_arStates[n]->GetCommand()) == 0)
        {
            g_nCurrentState = n;
            g_arStates[n]->Initialize(g_Leds, &g_CE); // Reinitialize 
            g_arStates[n]->CommandHandler(argc, argv);
            break;
        }
    }
}

//
// Add a new state to the state array
//
void addState(DisplayStateBase* pNewState)
{
    pNewState->Initialize(g_Leds, &g_CE);
    g_arStates[g_nStateCounter++] = pNewState;
    g_pConsole->Add(pNewState->GetCommand(), cmdStateChange, pNewState->GetCommandDescription());    
}

String getPage(){
    const char *s =    
    #include "..\www\index.html"
    ;    
    return String(s);
}

void rootPage()
{
    //char content[128];
    //sprintf(content, "<h1>Myriadclock</h1><br/><h2>Version %d</h2>", FIRMWARE_VERSION);    
    //g_server.send(200, "text/text", content);
    g_server.send(200, "text/html", getPage());
}

//
// Setup all hardware
//
void setup() 
{
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(g_Leds, NUM_LEDS); // Init of the Fastled library
	FastLED.setBrightness(100);

    g_Preferences.begin("Myriadclock", false);

    // Create + start the console
    g_pConsole = new Console(115200);   

    // Add all states to our statemachine
    addState(new DisplayStateBooting());
    addState(new DisplayStateClock());
    addState(new DisplayStateWords());
    addState(new DisplayStateUpdating());
    g_nCurrentState = 0;   

    g_server.on("/", rootPage);

    // Get CHIP ID:
    uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
	Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
	Serial.printf(" - %08X\n",(uint32_t)chipid);//print Low 4bytes.

    char buff[128];
    snprintf(buff, sizeof(buff), "Myriadclock-%04X", (uint16_t)(chipid & 0xFFFF));
    String clockName(buff);

    Serial.printf("Clockname: %s\n", clockName.c_str());

    AutoConnectConfig config; 
    config.apid = clockName;
    config.autoReconnect = true;
    config.retainPortal = true;
    config.autoSave = AC_SAVECREDENTIAL_AUTO;
    config.title = clockName;
    config.hostName = clockName;

    g_acPortal.config(config); 
    g_acPortal.begin();
    g_acUpdate.attach(g_acPortal);

    Serial.println("Webserver started: " + WiFi.localIP().toString());    
    
    timeClient.begin();
}


//
// Main program loop
//
void loop() 
{
    // TODO Make currentstate an ENUM
    if (g_nCurrentState < g_nStateCounter)
    {
        if (!g_arStates[g_nCurrentState]->HandleLoop(timeClient.getEpochTime()))
        {
            // When handleloop returns false, select the next handler
            g_nCurrentState = 1; // TODO always fallback to clock?
        }
    }

    // Allow the portal to handle stuff
    g_acPortal.handleClient();
        
    // And the console
    g_pConsole->Tick();    


    // Once every second, check the NTP stuff
    if (Elapsed(g_timestamp) > 1000)
    {                
        g_timestamp = millis();

        //Serial.println(WiFi.status());
        if (WiFi.status() == WL_CONNECTED)
        {
            if (!g_fNTPStarted)
            {
                timeClient.begin();
                g_fNTPStarted = true;
                Serial.println("NTP starting");

                AutoConnectCredential credential;
                station_config_t config;
                Serial.println("New credentials:");
                for(int i = 0; i < credential.entries(); i++) 
                {
                    credential.load(i, &config);
                    Serial.print("- Credentials for ");
                    Serial.print((char*) config.ssid);
                    Serial.print(" = ");
                    Serial.println((char*) config.password);    
                }
            }
        }
        else
        {
            // Show no-wifi   
        }

        if (g_fNTPStarted)
        {
            timeClient.update();
            //Serial.println(timeClient.getFormattedTime());

            // Check for 3->4 hour switch
            // Let Handle-Loop return another state
            // Convert to local time
            TimeChangeRule *tcr;    
            time_t t = g_CE.toLocal(timeClient.getEpochTime(), &tcr); // (Note: tcr cannot be NULL)
            int hours = hour(t);
            if (hours == 4 && g_nPreviousHour != 4)
            {
                // Switch to updating
                g_nCurrentState = 3;
            }
            g_nPreviousHour = hours;
        }
    }
}
