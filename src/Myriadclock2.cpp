/*  Myriadclock
    Using WS2812 LED Panel

    Created by Jochem Bakker

    This code is in the public domain...

    You can: copy it, use it, modify it, share it or just plain ignore it!
    Thx!

    NTPClient: https://github.com/arduino-libraries/NTPClient
        //Disabled:    Autoconnect stuff: https://github.com/Hieromon/AutoConnect
    TimeZone: https://github.com/JChristensen/Timezone
    Esp32FOTA: https://github.com/chrisjoyce911/esp32FOTA
    PILigt (ESPPiLight): https://github.com/puuu/ESPiLight

    TODO:
    HOMEKIT: https://github.com/maximkulkin/esp-homekit

    Bootstrap test page:
    https://juzraai.github.io/bootstrap4-test-page/

    s00500/ESPUI??
    Bootstrap: https://diyprojects.io/bootstrap-create-beautiful-web-interface-projects-esp8266/#.XkhzFWhKiUk

    Color picker:
    https://itsjavi.com/bootstrap-colorpicker/module-options.html

    Colors:
    https://sashat.me/2017/01/11/list-of-20-simple-distinct-colors/

    
*/

#include <NTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <time.h>
#include <Timezone.h>
//#include <AutoConnect.h>
//#include <ESPmDNS.h>

#include "Console.h"
#include "ClockLayout.h"
#define FASTLED_INTERNAL
#include "FastLED.h"       // Fastled library to control the LEDs

#include "DisplayStateBooting.h"
#include "DisplayStateWords.h"
#include "DisplayStateClock.h"
#include "DisplayStateUpdating.h"
#include "MyriadclockConfig.h"
#include "MyriadclockSettings.h"
#include "DisplayStateToilet.h"
#include "WebHandler.h"
#include <ESPiLight.h>
#include <ArduinoJson.h>

#include "MIOTConfigurator.h"


// Pin defines
#define DATA_PIN                13      // Connected to the data pin of the first LED strip
#define RECEIVER_PIN            22      // RXB6 input pin
#define TYPE_PIN                17      // Pin for determining the type of the board

using namespace std;

#define MAXSTATES               10      // Maximum number of LED states


static CRGB         g_Leds[NUM_LEDS]; // Define the array of leds
static Console*     g_pConsole = NULL;
static uint32_t     g_timestamp  = 0;
static DisplayStateBase* g_arStates[MAXSTATES] = {0};
static int          g_nStateCounter = 0;
static int          g_nCurrentState = 0;
static bool         g_fNTPStarted = false;
//static int          g_nPreviousHour = 0;

static MyriadclockSettings g_Settings;

WiFiUDP             g_ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and no offset
//NTPClient           g_timeClient(g_ntpUDP);

// Webservers
//WebServer           g_server;
MIOTConfigurator    g_miot(g_ntpUDP, "Myriadclock", FIRMWARE_VERSION);

//AutoConnect         g_acPortal(g_server);
//AutoConnectUpdate   g_acUpdate("www.myriadbits.com", 80);  // Step #3


// Central European Time (Frankfurt, Paris)
TimeChangeRule      CEST {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule      CET {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone            g_CE(CEST, CET);

// Create the webhandler
//WebHandler*        g_pWebHandler { NULL };

// RF433 Mhz receiver
ESPiLight          g_rf(-1);  // use -1 to disable transmitter

//
// Time elapsed??
//
uint32_t Elapsed(uint32_t ts)
{
    uint32_t now = xTaskGetTickCount();
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
            g_arStates[n]->Initialize(g_Leds, &g_CE, &g_Settings); // Reinitialize 
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
    pNewState->Initialize(g_Leds, &g_CE, &g_Settings);
    g_arStates[g_nStateCounter++] = pNewState;
    g_pConsole->Add(pNewState->GetCommand(), cmdStateChange, pNewState->GetCommandDescription());    
}

//
// Advertise presence using mDNS 
//
/*void AdvertiseServices(String myName)
{
    if (MDNS.begin(myName.c_str()))
    {
        Serial.println(F("mDNS responder started"));
        Serial.print(F("I am: "));
        Serial.println(myName.c_str());

        // Add service to MDNS-SD
        MDNS.addService("http", "tcp", 80);
        MDNS.addServiceTxt("_http", "_tcp", "Myriadclock", "3");
    }
    else
    {
        Serial.println(F("Error setting up MDNS responder"));
    }
}
*/



// callback function. It is called on successfully received and parsed rc signal
void rfCallback(const String &protocol, const String &message, int status, size_t repeats, const String &deviceID) 
{
    Serial.print("RF signal arrived [");
    Serial.print(protocol);  // protocoll used to parse
    Serial.print("][");
    Serial.print(deviceID);  // value of id key in json message
    Serial.print("] (");
    Serial.print(status);  // status of message, depending on repeat, either:
                        // FIRST   - first message of this protocoll within the
                        //           last 0.5 s
                        // INVALID - message repeat is not equal to the
                        //           previous message
                        // VALID   - message is equal to the previous message
                        // KNOWN   - repeat of a already valid message
    Serial.print(") ");
    Serial.print(message);  // message in json format
    Serial.println();

    // check if message is valid and process it
    if (status == VALID) {
        Serial.print("Valid message: [");
        Serial.print(protocol);
        Serial.print("] ");
        Serial.print(message);
        Serial.println();

        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, message);
        if (!error)
        {
            const char* s = doc["state"];
            if (strcasecmp(s, "opened") == 0)
            {
                Serial.println("--OPENED");
                g_Settings.colTime = 0x00FF00;
            }
            if (strcasecmp(s, "closed") == 0)
            {
                Serial.println("--CLOSED");
                g_Settings.colTime = 0xFF0000;
            }
        }
        
    }
}


//
// Setup all hardware
//
void setup() 
{
    // Create + start the console
    g_pConsole = new Console(115200);   

    // Determine the board type/capabilities
    pinMode(TYPE_PIN, INPUT_PULLDOWN);  // set pin as input
    //Serial.printf("Type: %d\n", digitalRead(TYPE_PIN)); 

	FastLED.addLeds<NEOPIXEL, DATA_PIN>(g_Leds, NUM_LEDS); // Init of the Fastled library
	FastLED.setBrightness(100);    

    // Load/initialize all settings
    g_Settings.Initialize();

    // Get CHIP ID:
    uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
	//Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid >> 32));//print High 2 bytes
	//Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.

    // Get the word codes
    uint16_t chipNumber = (uint16_t)((chipid >> 32) & 0xFFFF) ^ (uint16_t)((chipid >> 16) & 0xFFFF) ^ (uint16_t)(chipid & 0xFFFF);
    //Serial.printf("SerialNumber: %d\n", chipNumber);
    
    // Fabricate the clock name
    char buff[128];
    snprintf(buff, sizeof(buff), "Myriadclock%s", s_wordCodes[chipNumber % 32].text);

    // Put name + serial number in settings (volatile, not store to DB)
    g_Settings.sClockName = String(buff);
    g_Settings.nSerialNumber = chipNumber;

    //Serial.printf("Type: %d\n", digitalRead(TYPE_PIN)); 

   //Sset callback function
    g_rf.setCallback(rfCallback);

    // Initialize receiver
    g_rf.initReceiver(RECEIVER_PIN);

    //Serial.printf("Type: %d\n", digitalRead(TYPE_PIN)); 

    // Initialize/start the webhandler
   // g_pWebHandler = new WebHandler(g_server, g_Settings);
    //Serial.printf("Type: %d\n", digitalRead(TYPE_PIN)); 

    // Add all states to our statemachine
    addState(new DisplayStateBooting());
    addState(new DisplayStateClock());
    addState(new DisplayStateWords());
    addState(new DisplayStateUpdating());
    addState(new DisplayStateToilet());
    g_nCurrentState = 0;   

    //Serial.printf("Type: %d\n", digitalRead(TYPE_PIN)); 

    //AutoConnectConfig config; 
    //config.apid = g_Settings.sClockName;
    //config.autoReconnect = true;
    //config.retainPortal = true;
    //config.autoSave = AC_SAVECREDENTIAL_AUTO;
    //config.title = g_Settings.sClockName;
    //config.hostName = g_Settings.sClockName;

    //g_acPortal.config(config); 
    //g_acPortal.begin();
    //g_acUpdate.attach(g_acPortal);

    g_miot.setup();

    //Serial.println("Webserver started: " + WiFi.localIP().toString());    
    //Serial.printf("Type: %d\n", digitalRead(TYPE_PIN)); 

    //AdvertiseServices(g_Settings.sClockName);
}


//
// Main program loop
//
void loop() 
{
    // Handle RF 433 Mhz
    g_rf.loop();   

    // TODO Make currentstate an ENUM
    if (g_nCurrentState < g_nStateCounter)
    {
        if (!g_arStates[g_nCurrentState]->HandleLoop(0))// g_timeClient.getEpochTime()))
        {
            // When handleloop returns false, select the next handler
            g_nCurrentState = 0; // TODO always fallback to clock?
        }
    }

    // Allow the portal to handle stuff
    //g_acPortal.handleClient();
    g_miot.handleClient();

    //g_server.handleClient();
        
    // And the console
    g_pConsole->Tick();    

    // Once every x seconds, check the NTP stuff
    if (Elapsed(g_timestamp) > 1000)
    {                
        g_timestamp = xTaskGetTickCount();

        //Serial.println(WiFi.status());
        if (WiFi.status() == WL_CONNECTED)
        {
            g_nCurrentState = 4; // Toilet

            if (!g_fNTPStarted)
            {
              //  g_timeClient.begin();
                g_fNTPStarted = true;
                Serial.println("NTP starting");

                //AutoConnectCredential credential;
                //station_config_t config;
                //Serial.println("New credentials:");
                //for(int i = 0; i < credential.entries(); i++) 
                //{
                    //credential.load(i, &config);
                    //Serial.print("- Credentials for ");
                    //Serial.print((char*) config.ssid);
                    //Serial.print(" = ");
                    //Serial.println((char*) config.password);    
                //}
            }
        }
        else
        {
            // Show no-wifi   
            g_nCurrentState = 0; // Booting
        }

        if (g_fNTPStarted)
        {
        //    g_timeClient.update();
            //Serial.println(timeClient.getFormattedTime());

            // Check for 3->4 hour switch
            // Let Handle-Loop return another state
            // Convert to local time
            // TimeChangeRule *tcr;    
            // time_t t = g_CE.toLocal(g_timeClient.getEpochTime(), &tcr); // (Note: tcr cannot be NULL)
            // int hours = hour(t);
            // if (hours == 4 && g_nPreviousHour != 4)
            // {
            //     // Switch to updating
            //     g_nCurrentState = 3;
            // }
            // g_nPreviousHour = hours;
        }
    }   
}
