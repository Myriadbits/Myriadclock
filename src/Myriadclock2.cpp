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
#define FASTLED_INTERNAL
#include <FastLED.h>       // Fastled library to control the LEDs

#include "DisplayStateManager.h"
#include "MyriadclockConfig.h"
#include "MyriadclockSettings.h"
#include "WebHandler.h"
//#include <ESPiLight.h>
#include <ArduinoJson.h>

#include "MIOTConfigurator.h"


// Pin defines
#define DATA_PIN                13      // Connected to the data pin of the first LED strip
#define RECEIVER_PIN            22      // RXB6 input pin
#define TYPE_PIN                17      // Pin for determining the type of the board

using namespace std;


static CRGB         g_Leds[NUM_LEDS]; // Define the array of leds
static uint32_t     g_timestamp  = 0;
static bool         g_fNTPStarted = false;
static int          g_nPreviousHour = 0;

static MyriadclockSettings g_Settings;

WiFiUDP             g_ntpUDP;

// Manager containing all display state related functionality
DisplayStateManager g_stateManager;

// By default 'pool.ntp.org' is used with 60 seconds update interval and no offset
NTPClient           g_timeClient(g_ntpUDP, "pool.ntp.org");

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
//ESPiLight          g_rf(-1);  // use -1 to disable transmitter

//
// Time elapsed??
//
uint32_t Elapsed(uint32_t ts)
{
    uint32_t now = xTaskGetTickCount();
    if (ts < now) return (now - ts);
    return(now + (0xffffffff - ts));
}


// callback function. It is called on successfully received and parsed rc signal
// void rfCallback(const String &protocol, const String &message, int status, size_t repeats, const String &deviceID) 
// {
//     Serial.print("RF signal arrived [");
//     Serial.print(protocol);  // protocoll used to parse
//     Serial.print("][");
//     Serial.print(deviceID);  // value of id key in json message
//     Serial.print("] (");
//     Serial.print(status);  // status of message, depending on repeat, either:
//                         // FIRST   - first message of this protocoll within the
//                         //           last 0.5 s
//                         // INVALID - message repeat is not equal to the
//                         //           previous message
//                         // VALID   - message is equal to the previous message
//                         // KNOWN   - repeat of a already valid message
//     Serial.print(") ");
//     Serial.print(message);  // message in json format
//     Serial.println();

//     // check if message is valid and process it
//     if (status == VALID) {
//         Serial.print("Valid message: [");
//         Serial.print(protocol);
//         Serial.print("] ");
//         Serial.print(message);
//         Serial.println();

//         StaticJsonDocument<200> doc;
//         DeserializationError error = deserializeJson(doc, message);
//         if (!error)
//         {
//             const char* s = doc["state"];
//             if (strcasecmp(s, "opened") == 0)
//             {
//                 Serial.println("--OPENED");
//                 g_Settings.colTime = 0x00FF00;
//             }
//             if (strcasecmp(s, "closed") == 0)
//             {
//                 Serial.println("--CLOSED");
//                 g_Settings.colTime = 0xFF0000;
//             }
//         }
        
//     }
// }


//
// Setup all hardware
//
void setup() 
{
    // Create + start the console
    Console::getInstance().start(115200);   

    // Determine the board type/capabilities
    pinMode(TYPE_PIN, INPUT_PULLDOWN);  // set pin as input
    //Serial.printf("Type: %d\n", digitalRead(TYPE_PIN)); 

	FastLED.addLeds<NEOPIXEL, DATA_PIN>(g_Leds, NUM_LEDS); // Init of the Fastled library
	FastLED.setBrightness(50);    

    // Load/initialize all settings
    g_Settings.Initialize();

    // Initialize all display related functionality
    g_stateManager.initialize(g_Leds, &g_CE, &g_Settings);

    // Get CHIP ID:
    uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
	//Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid >> 32));//print High 2 bytes
	//Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.

    // Get the word codes
    uint16_t chipNumber = (uint16_t)((chipid >> 32) & 0xFFFF) ^ (uint16_t)((chipid >> 16) & 0xFFFF) ^ (uint16_t)(chipid & 0xFFFF);
    //Serial.printf("SerialNumber: %d\n", chipNumber);
    
    // Fabricate the clock name
    char buff[128];
   // snprintf(buff, sizeof(buff), "Myriadclock%s", s_wordCodes[chipNumber % 32].text); TODO

    // Put name + serial number in settings (volatile, not store to DB)
    g_Settings.sClockName = String(buff);
    g_Settings.nSerialNumber = chipNumber;

    //Serial.printf("Type: %d\n", digitalRead(TYPE_PIN)); 

   //Sset callback function
    //g_rf.setCallback(rfCallback);

    // Initialize receiver
    //g_rf.initReceiver(RECEIVER_PIN);

    //Serial.printf("Type: %d\n", digitalRead(TYPE_PIN)); 

    // Initialize/start the webhandler
   // g_pWebHandler = new WebHandler(g_server, g_Settings);
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

    g_miot.setup(&g_stateManager);
    g_miot.addConfigItem(1, CT_WIFI_SSID, "SSID", "WiFi SSID", "");
    g_miot.addConfigItem(2, CT_WIFI_PASSWORD, "Passphrase", "WiFi Passphrase", "");
    g_miot.addConfigItem(10, CT_RGBCOLOR, "Time color", "Color of the hours/minutes part", "");
    g_miot.addConfigItem(11, CT_RGBCOLOR, "Weekday Color", "Color of the day of the week", "");
    g_miot.addConfigItem(12, CT_RGBCOLOR, "Date color", "Color of the date part", "");


    //Serial.println("Webserver started: " + WiFi.localIP().toString());    
    //Serial.printf("Type: %d\n", digitalRead(TYPE_PIN)); 
}


//
// Main program loop
//
void loop() 
{
    // Handle RF 433 Mhz
    //g_rf.loop();   

    // TODO Make currentstate an ENUM
    g_stateManager.handleLoop(g_timeClient.getEpochTime());

    // Allow the portal to handle stuff
    //g_acPortal.handleClient();
    g_miot.handleClient();

    //g_server.handleClient();
        
    // And the console
    Console::getInstance().tick();    

    // Show no-wifi while there is still no Wifi  
    if (WiFi.status() != WL_CONNECTED)
    {
        // When no other state is active, fallback to the no-wifi state
        g_stateManager.setDefaultState(DS_NOWIFI);
    }

    // Once every x seconds, check the NTP stuff
    if (Elapsed(g_timestamp) > 1000)
    {                
        g_timestamp = xTaskGetTickCount();

        //Serial.println(WiFi.status());
        if (WiFi.status() == WL_CONNECTED)
        {
            if (!g_fNTPStarted)
            {
                g_stateManager.changeState(DS_BOOTING); // Booting
                g_timeClient.begin();
                g_fNTPStarted = true;
                Serial.println("NTP starting");
            }
        }

        if (g_fNTPStarted)
        {
            g_timeClient.update();
            
            // Check for 3->4 hour switch
            // Let Handle-Loop return another state
            // Convert to local time
            TimeChangeRule *tcr;    
            time_t t = g_CE.toLocal(g_timeClient.getEpochTime(), &tcr); // (Note: tcr cannot be NULL)
            int hours = hour(t);
            int currentYear = year(t);

            if (currentYear > 1970 && g_nPreviousHour != hours)
            {
                g_stateManager.setDefaultState(DS_CLOCK);
            }

            if (hours == 4 && g_nPreviousHour != 4)
            {
                // Switch to updating
                g_stateManager.changeState(DS_UPDATING);
            }
            g_nPreviousHour = hours;
        }
    }   
}
