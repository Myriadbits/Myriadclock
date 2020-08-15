
#include "MIOTConfigurator.h"

//
// Constructor, use this to change the timeouts
// MIOTConfigurator uses SmartConfig create a WiFi connection, upon connection losses
// and incorrect credentials the SmartConfig procedure will be restarted. When SmartConfig
// times-out, stored WiFi credentials will be retried.
// 
// @param wifiConnectionTimeout Timeout for connecting to the WiFi
// @param wifiConnectionLostTimeout Timeout for restarting the SmartConfig when an existing WiFi connection is lost
// @param smartConfigTimeout Timeout for retrying the WiFi credentials (if any) when the SmartConfig does not receive credentials
MIOTConfigurator::MIOTConfigurator(WiFiUDP& udp, String productName, int version)
    : m_state(MIOTState_Unconnected)
    , m_wifiConnectionTimeout(MIOT_TIMEOUT_WIFICONNECTION)
    , m_wifiConnectionLostTimeout(MIOT_TIMEOUT_WIFICONNECTIONLOST)    
    , m_wifiAPSettingsTimeout(MIOT_TIMEOUT_SMARTCONFIG) // TODO Increase to 15 min?
    , m_productName(productName)
    , m_version(version)
    , m_clients(-1)
    //, m_server(MIOT_AP_SETTINGS_PORT)
{
    m_pUDP = &udp;
}

//
// Private method to change the state of our main statemachine
// @param newState The new state
void MIOTConfigurator::changeState(EMIOTState newState)
{
    m_state = newState;
    m_millisLastStateChange = millis();
}

//
// Legacy event handler, might be required for mDNS
//esp_err_t legacy_event_handler(void *ctx, system_event_t *event) 
//{
    //mdns_handle_system_event(ctx, event);
    //MIOT_LOG("Event handler\n");
  //  return ESP_OK;
//}
//static void _on_sys_event(system_event_t *event)
//{
  //  mdns_handle_system_event(NULL, event);
//}

//
// setup the MIOT Configurator, call this method in the setup function
// This setup will:
// - Start WiFi using the stored credentials (if any and if valid)
// - If no credentials are found, SmartConfig is started
void MIOTConfigurator::setup(unsigned long wifiConnectionTimeout, unsigned long wifiConnectionLostTimeout, unsigned long wifiAPSettingTimeout)
{
    m_wifiConnectionTimeout = wifiConnectionTimeout;
    m_wifiConnectionLostTimeout = wifiConnectionLostTimeout;   
    m_wifiAPSettingsTimeout = wifiAPSettingTimeout; 

    // See statemachine in handlelient
    m_state = MIOTState_Unconnected;

    // Default device ID is the serial number of the Chip
    if (m_deviceId.isEmpty())
    {
        uint64_t chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
        // Chip ID is 64 bit, I find that a bit large for an ID, make it 16 bit (I know, there is a chance some are the same)
        m_deviceId = String((uint16_t)((chipid >> 32) & 0xFFFF) ^ (uint16_t)((chipid >> 16) & 0xFFFF) ^ (uint16_t)(chipid & 0xFFFF), 16);
        m_deviceId.toUpperCase();
    }
    m_deviceName = m_productName + "-" + m_deviceId;
    WiFi.setHostname(m_deviceName.c_str());

    MIOT_LOG("Starting MIOT Configurator\n");
    MIOT_LOG("- Product:  %s\n", m_productName.c_str());
    MIOT_LOG("- DeviceId: %s\n", m_deviceId.c_str());
    MIOT_LOG("- DeviceName: %s\n", m_deviceName.c_str());
    MIOT_LOG("- Version:  %d\n", m_version);
}

//
// Advertise presence using mDNS 
//
// void MIOTConfigurator::advertise()
// {
//     esp_err_t err = mdns_init();
//     if (err) 
//     {
//         MIOT_LOG("Error setting up mDNS: %d\n", err);
//         return;
//     }

//     MIOT_LOG("mDNS started: `%s.%s.%s`\n", m_deviceName.c_str(), MIOT_MDNS_SERVER, MIOT_MDNS_PROTO);

//     //set hostname
//     mdns_hostname_set(m_deviceName.c_str());

//     //set default instance
//     mdns_instance_name_set(m_deviceName.c_str());

//     //_services._dns-sd._udp
//     mdns_service_add(NULL, MIOT_MDNS_SERVER, MIOT_MDNS_PROTO, MIOT_MDNS_PORT, NULL, 0);
//     //mdns_service_add("_services", "_dns-sd", "_udp", 80, NULL, 0);
//     mdns_service_txt_item_set(MIOT_MDNS_SERVER, MIOT_MDNS_PROTO, "product", m_productName.c_str());
//     mdns_service_txt_item_set(MIOT_MDNS_SERVER, MIOT_MDNS_PROTO, "deviceid", m_deviceId.c_str());
//     mdns_service_txt_item_set(MIOT_MDNS_SERVER, MIOT_MDNS_PROTO, "version", String(m_version).c_str());
//     mdns_service_txt_item_set(MIOT_MDNS_SERVER, MIOT_MDNS_PROTO, "lifetime", String(millis()).c_str());

//     MIOT_LOG("DeviceURL: http://%s.local:%d\n", m_deviceName.c_str(), MIOT_MDNS_PORT);    
// }


//
// handleClient should be called in the main loop repeately
// This method will handle the connection statemachine
// - When WiFi connection times-out, the smart config is started
// - If the credentials that we receive using smart config does not work, smart config is restarted
// - Upon Wifi connection loss (using a large timeout), still not working => start smart config
// - No Wifi and still waiting for smart config, try the stored credetials every x minutes
void MIOTConfigurator::handleClient()
{
    switch (m_state)
    {
    case MIOTState_Unconnected:
        if (m_preferences.begin(MIOT_PREF_CONFIG, false)) // Open storage
        {
            String ssid = m_preferences.getString("ssid", "");
            if (false) // (!ssid.isEmpty())
            {
                String passphrase = m_preferences.getString("passphrase", "");
                MIOT_LOG("Found credentials, trying to connect to WiFi '%s'\n", ssid.c_str());

                // Yes, start connecting using these settings
                WiFi.begin(ssid.c_str(), passphrase.c_str());
                changeState(MIOTState_WaitingForWifi);
            }
            else
            {
                changeState(MIOTState_StartingAPSettings);
            }
            m_preferences.end();
        }
        break;

    case MIOTState_StartingAPSettings:
        //WiFi.softAPdisconnect();
           //WiFi.softAPsetHostname(m_deviceName.c_str());
        if (WiFi.softAP(m_deviceName.c_str(), "1234bier", 1))
        {                
            delay(100);
            if (WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0)))
            {
                //m_server.begin();
                MIOT_LOG("Start listening for SoftAP Setting\n");
                MIOT_LOG("- SoftAP IP Address:  %s\n", WiFi.softAPIP().toString().c_str());

                if (m_pUDP)
                {
                    if (!m_pUDP->begin(MIOT_AP_SETTINGS_PORT))
                    {
                        MIOT_LOG("- Failed to open UDP port!\n");    
                    }
                    MIOT_LOG("- SoftAP IP Port:  %d\n", MIOT_AP_SETTINGS_PORT);
                    MIOT_LOG("Waiting for SoftAP settings.\n");                  
                    
                    changeState(MIOTState_WaitingForAPSettings);
                }
                else
                {
                    // Hmm, don't have udp, so try to connect to wifi again??
                    // Actually I don't know what to do now :(
                    changeState(MIOTState_WaitingForWifi);
                }
            }
        }
        //else
        //{
        //    MIOT_LOG("Failed to create the soft AP!\n");
        //}
        break;

    case MIOTState_WaitingForAPSettings:
        // When we do not receive smart config message, retry the stored credentials (if any)
        if ((millis() - m_millisLastStateChange) > m_wifiAPSettingsTimeout)
        {
            MIOT_LOG("APSettings timed-out. Retrying WiFi credentials\n");
            changeState(MIOTState_Unconnected);
        }
        else if (m_pUDP)
        {
            if (WiFi.softAPgetStationNum() != m_clients)
            {
                m_clients = WiFi.softAPgetStationNum();
                MIOT_LOG("SoftAP number of clients: %d.\n", m_clients);
            }

            // WiFiClient client = m_server.available();
            // if (client.connected()) 
            // { 
            //     MIOT_LOG("Client connected: %s.\n", client.remoteIP().toString().c_str());
            //     while (client.connected()) 
            //     {
            //         int data = client.read();
            //         if (data > 0) 
            //         {
            //             Serial.println(data);
            //         } 
            //         delay(10); // Delay needed for receiving the data correctly
            //     } 
            //     Serial.println("The client disconnected");
            //     delay(100);
            // }
 
  
            int dataSize = m_pUDP->parsePacket();
            if (dataSize > 0) 
            {
                MIOT_LOG("Received %d bytes from %s\n", dataSize, m_pUDP->remoteIP().toString().c_str());

                // read the packet into packetBufffer
                char packetBuffer[2000]; //buffer to hold incoming packet,
                m_pUDP->read(packetBuffer, 2000);
                packetBuffer[dataSize] = 0;
                MIOT_LOG("Contents: %s.\n", packetBuffer);
                
                //changeState(MIOTState_WaitingForWifi); // Smart config succeeded, now trying to connect to Wifi
                //MIOT_LOG("SmartConfig credentials received. Waiting for WiFi.\n");
            }
        }
        break;

    case MIOTState_WaitingForWifi:
        // Our we connected to Wifi yet?
        if (WiFi.status() == WL_CONNECTED)
        {
            MIOT_LOG("WiFi connected to %s\n", WiFi.SSID().c_str());
            MIOT_LOG("- IP Address:  %s\n", WiFi.localIP().toString().c_str());
            MIOT_LOG("- Subnet mask: %s\n", WiFi.subnetMask().toString().c_str());
            MIOT_LOG("- Gateway:     %s\n", WiFi.gatewayIP().toString().c_str());
            
            // Start advertising our name
            //advertise();

            //This initializes udp and transfer buffer
            // if (m_pUDP != nullptr)
            // {
            //     int status = m_pUDP->begin(23122);
            //     MIOT_LOG("- Started UDP: %d\n", status);
            // }

            // Store the WiFi credentials
            if (m_preferences.begin(MIOT_PREF_CONFIG, false)) // Open storage
            {
                m_preferences.putString("ssid", WiFi.SSID());
                m_preferences.putString("passphrase", WiFi.psk());
                m_preferences.end();
            }

            changeState(MIOTState_Connected);
        }
        else
        {
            // Check timeout
            if ((millis() - m_millisLastStateChange) > m_wifiConnectionTimeout)
            {
                MIOT_LOG("WiFi connection timeout. Restarting SmartConfig\n");
                changeState(MIOTState_StartingAPSettings); // This takes too long, jump back to waiting for smartconfig
            }
        }
        break;

    case MIOTState_Connected:
        if (WiFi.status() != WL_CONNECTED)
        {
            MIOT_LOG("WiFi connection lost\n");
            changeState(MIOTState_ConnectionLost);
        }

        //if (m_pUDP != nullptr)
        //{
        //    int packetSize = m_pUDP->parsePacket();
        //    if (packetSize) 
        //    {
        //        MIOT_LOG("Received packet of size %d\n", packetSize);
        //        MIOT_LOG("From %s port %d\n", m_pUDP->remoteIP().toString().c_str(), m_pUDP->remotePort() );
        //
        //    }
        //    m_pUDP->flush();
        //}
        
        // if ((millis() - m_millisLastUDP) > 1000)
        // {
        //     const char *ptext = "Hallo jochem";
        //     m_udp.beginPacket(WiFi.broadcastIP(), 2323);
        //     m_udp.write((const uint8_t*)ptext, strlen(ptext));
        //     m_udp.endPacket();
        //     m_millisLastUDP = millis();
        // }

        // Trick to keep updating the announcement:
       // mdns_service_txt_item_set(MIOT_MDNS_SERVER, MIOT_MDNS_PROTO, "product", m_productName.c_str());
        //mdns_service_txt_item_set(MIOT_MDNS_SERVER, MIOT_MDNS_PROTO, "deviceid", m_deviceId.c_str());
        //mdns_service_txt_item_set(MIOT_MDNS_SERVER, MIOT_MDNS_PROTO, "version", String(m_version).c_str());
        //mdns_service_txt_item_set(MIOT_MDNS_SERVER, MIOT_MDNS_PROTO, "lifetime", String(millis()).c_str());

        //{
        //    String hostName = m_productName + "_" + m_deviceId;
        //    mdns_instance_name_set(hostName.c_str());
        //}

        break;

    case MIOTState_ConnectionLost:
        if (WiFi.status() == WL_CONNECTED)
        {
            MIOT_LOG("WiFi connection re-established\n");
            changeState(MIOTState_Connected);
        }
        else
        {
            // Check timeout
            if ((millis() - m_millisLastStateChange) > m_wifiConnectionLostTimeout)
            {
                // This takes too long, jump back to waiting for smartconfig
                MIOT_LOG("WiFi connection timeout due to lost connection. Restarting SmartConfig\n");
                changeState(MIOTState_StartingAPSettings);

                // Stop the MDNS
    //            MDNS.end();
            }
        }
        break;

    default: // Should never occur
        changeState(MIOTState_Unconnected);
        break;
    }
}
