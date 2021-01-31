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

    TODO:
    HOMEKIT: https://github.com/maximkulkin/esp-homekit

    Colors:
    https://sashat.me/2017/01/11/list-of-20-simple-distinct-colors/
*/

#include <NTPClient.h>
#include <WiFi.h>
#include <time.h>
#include <Timezone.h>

#include "..\lib\BLEConfig\include\BLEConfig.h"

#include "Console.h"
#define FASTLED_INTERNAL
#include <FastLED.h>       // Fastled library to control the LEDs

#include "DisplayStateManager.h"
#include "MyriadclockConfig.h"
#include <ArduinoJson.h>

#define MYRIADCLOCK_MANUFACTURER    "Myriadbits"
#define MYRIADCLOCK_MODEL           "Myriadclock"
#define MYRIADCLOCK_VERSION         "1.0.1"


// Pin defines
#define DATA_PIN                13      // Connected to the data pin of the first LED strip
#define RECEIVER_PIN            22      // RXB6 input pin
#define TYPE_PIN                17      // Pin for determining the type of the board

using namespace std;


static CRGB         g_Leds[NUM_LEDS]; // Define the array of leds
static uint32_t     g_timestamp  = 0;
static bool         g_fNTPStarted = false;
static int          g_nPreviousHour = 0;

WiFiUDP             g_ntpUDP;

// Manager containing all display state related functionality
DisplayStateManager g_stateManager;

// By default 'pool.ntp.org' is used with 60 seconds update interval and no offset
NTPClient           g_timeClient(g_ntpUDP, "pool.ntp.org");

// The BLE Config library
BLEConfig           g_bleconfig(MYRIADCLOCK_MODEL, MYRIADCLOCK_MANUFACTURER, MYRIADCLOCK_VERSION, 256); // 256 = Clock TODO VERSION

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


    // Load/initialize all BLE Config settings
    g_bleconfig.registerWifi(CONFIG_WIFI, "WiFi SSID");

    BLEConfigItemOption *pconfig = g_bleconfig.registerOption(CONFIG_LAYOUT, "Clock layout", 0);
    pconfig->addOption((uint8_t) 0, "Dutch V2");
    pconfig->addOption((uint8_t) 1, "Dutch V1");
    pconfig->addOption((uint8_t) 2, "English"); 

    pconfig = g_bleconfig.registerOption(CONFIG_DAYLIGHTSAVING, "Daylight saving time", 0);
    pconfig->addOption((uint8_t) 0, "Off"); 
    pconfig->addOption((uint8_t) 1, "Central European"); 
    pconfig->addOption((uint8_t) 2, "United Kingdom"); 
    pconfig->addOption((uint8_t) 3, "Australia");
    pconfig->addOption((uint8_t) 4, "US"); 
      
    pconfig = g_bleconfig.registerOption(CONFIG_TIMEZONE, "Timezone", 13);
    pconfig->addOption((uint8_t) 0, "-12"); 
    pconfig->addOption((uint8_t) 1, "-11"); 
    pconfig->addOption((uint8_t) 2, "-10");
    pconfig->addOption((uint8_t) 3, "-9");
    pconfig->addOption((uint8_t) 4, "-8"); 
    pconfig->addOption((uint8_t) 5, "-7"); 
    pconfig->addOption((uint8_t) 6, "-6"); 
    pconfig->addOption((uint8_t) 7, "-5"); 
    pconfig->addOption((uint8_t) 8, "-4"); 
    pconfig->addOption((uint8_t) 9, "-3"); 
    pconfig->addOption((uint8_t) 10, "-2"); 
    pconfig->addOption((uint8_t) 11, "-1"); 
    pconfig->addOption((uint8_t) 12, "0"); 
    pconfig->addOption((uint8_t) 13, "1"); 
    pconfig->addOption((uint8_t) 14, "2"); 
    pconfig->addOption((uint8_t) 15, "3"); 
    pconfig->addOption((uint8_t) 16, "4"); 
    pconfig->addOption((uint8_t) 17, "5"); 
    pconfig->addOption((uint8_t) 18, "6"); 
    pconfig->addOption((uint8_t) 19, "7"); 
    pconfig->addOption((uint8_t) 20, "8"); 
    pconfig->addOption((uint8_t) 21, "9"); 
    pconfig->addOption((uint8_t) 22, "10"); 
    pconfig->addOption((uint8_t) 23, "11"); 
    pconfig->addOption((uint8_t) 24, "12"); 

    g_bleconfig.registerRGBColor(CONFIG_COLOR_TIME, "Time color", 0x00FF00, true, "Color of the hours/minutes part");
    g_bleconfig.registerRGBColor(CONFIG_COLOR_WEEKDAY, "Weekday Color", 0xFFA500, true, "Color of the day of the week");
    g_bleconfig.registerRGBColor(CONFIG_COLOR_DATE, "Date color", 0xE59400, true, "Color of the date part");

    g_bleconfig.registerSlider(CONFIG_BRIGHTNESS_DAY, "Brightness Day", 80, false, "Brightness during the day");
    g_bleconfig.registerSlider(CONFIG_BRIGHTNESS_NIGHT, "Brightness Night", 30, false, "Brightness during the night");
    g_bleconfig.registerSlider(CONFIG_BRIGHTNESS_BACKGROUND, "Background Brightness", 4, false, "Brightness of the background");

    pconfig = g_bleconfig.registerOption(CONFIG_OPTIONS_TIME, "Time color options", DO_STATIC);
    pconfig->addOption((uint8_t) DO_STATIC, "Manual");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLENORMAL, "Color cycle");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLEHOUR, "Color cycle hourly");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_AYURVEDA, "Ayurveda colors daily");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_THAI, "Thai colors daily");

    pconfig = g_bleconfig.registerOption(CONFIG_OPTIONS_WEEKDAY, "Weekday color options", DO_STATIC);
    pconfig->addOption((uint8_t) DO_STATIC, "Manual");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLENORMAL, "Color cycle");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLEHOUR, "Color cycle hourly");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_AYURVEDA, "Ayurveda colors daily");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_THAI, "Thai colors daily");

    pconfig = g_bleconfig.registerOption(CONFIG_OPTIONS_DATE, "Date color options", DO_STATIC);
    pconfig->addOption((uint8_t) DO_STATIC, "Manual");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLENORMAL, "Color cycle");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLEHOUR, "Color cycle hourly");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_AYURVEDA, "Ayurveda colors daily");
    pconfig->addOption((uint8_t) DO_COLOR_WEEK_THAI, "Thai colors daily");

    pconfig = g_bleconfig.registerOption(CONFIG_OPTIONS_SPECIAL, "Special day options", DO_COLOR_PARTY_QUICK);
    pconfig->addOption((uint8_t) DO_COLOR_CYCLENORMAL, "Color cycle");
    pconfig->addOption((uint8_t) DO_COLOR_CYCLEHOUR, "Color cycle hourly");
    pconfig->addOption((uint8_t) DO_COLOR_PARTY_SLOW, "Party colors slow");
    pconfig->addOption((uint8_t) DO_COLOR_PARTY_QUICK, "Party colors quick");
    pconfig->addOption((uint8_t) DO_COLOR_PARTY_MINUTE, "Party colors minute");

    g_bleconfig.registerDate(CONFIG_BIRTHDAY_1, "Birthday 1", 1971, 12, 23, false);
    g_bleconfig.registerDate(CONFIG_BIRTHDAY_2, "Birthday 2", 2021, 1, 31, false);
    g_bleconfig.registerDate(CONFIG_BIRTHDAY_3, "Birthday 3", 0, 0, 0, false);
    g_bleconfig.registerDate(CONFIG_BIRTHDAY_4, "Birthday 4", 0, 0, 0, false);

    // Start the BLE Config stuff
    // This will also load all previously stored settings
    g_bleconfig.start(&g_stateManager);

    // Initialize all display related functionality
    g_stateManager.initialize(g_Leds, &g_bleconfig);
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
            }
        }

        if (g_fNTPStarted)
        {
            g_timeClient.update();
            
            // Check for 3->4 hour switch
            // Let Handle-Loop return another state
            // Convert to local time
            TimeChangeRule *tcr;    
            TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};     // UTC
            Timezone UTC(utcRule);
            time_t t = UTC.toLocal(g_timeClient.getEpochTime(), &tcr); // (Note: tcr cannot be NULL)
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
