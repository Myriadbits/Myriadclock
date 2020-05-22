
#include "MIOTConfigurator.h"
//#include "esp_smartconfig.h"

//
// Constructor, use this to change the timeouts
// MIOTConfigurator uses SmartConfig create a WiFi connection, upon connection losses
// and incorrect credentials the SmartConfig procedure will be restarted. When SmartConfig
// times-out, stored WiFi credentials will be retried.
// 
// @param wifiConnectionTimeout Timeout for connecting to the WiFi
// @param wifiConnectionLostTimeout Timeout for restarting the SmartConfig when an existing WiFi connection is lost
// @param smartConfigTimeout Timeout for retrying the WiFi credentials (if any) when the SmartConfig does not receive credentials
MIOTConfigurator::MIOTConfigurator(String productName, int version)
    : m_state(MIOTState_Unconnected)
    , m_wifiConnectionTimeout(MIOT_TIMEOUT_WIFICONNECTION)
    , m_wifiConnectionLostTimeout(MIOT_TIMEOUT_WIFICONNECTIONLOST)
    , m_smartConfigTimeout(MIOT_TIMEOUT_SMARTCONFIG) // TODO Increase to 15 min?
    , m_productName(productName)
    , m_version(version)
{
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
// setup the MIOT Configurator, call this method in the setup function
// This setup will:
// - Start WiFi using the stored credentials (if any and if valid)
// - If no credentials are found, SmartConfig is started
void MIOTConfigurator::setup(unsigned long wifiConnectionTimeout, unsigned long wifiConnectionLostTimeout, unsigned long smartConfigTimeout)
{
    m_wifiConnectionTimeout = wifiConnectionTimeout;
    m_wifiConnectionLostTimeout = wifiConnectionLostTimeout;
    m_smartConfigTimeout = smartConfigTimeout;

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
    MIOT_LOG("Starting MIOT Configurator\n");
    MIOT_LOG("- Product:  %s\n", m_productName.c_str());
    MIOT_LOG("- DeviceId: %s\n", m_deviceId.c_str());
    MIOT_LOG("- Version:  %d\n", m_version);
}

//
// Advertise presence using mDNS 
//
void MIOTConfigurator::advertise()
{
    String hostName = m_productName + "_" + m_deviceId;
    WiFi.setHostname(hostName.c_str());
    if (MDNS.begin(hostName.c_str()))
    {
        MIOT_LOG("mDNS started, hostname: `%s`\n", hostName.c_str());

        MDNS.setInstanceName(hostName.c_str());
        MDNS.addService("_dns-sd", "_udp", 80);
        //\x09_services\x07_dns-sd\x04_udp

        // Add service to MDNS-SD
        MDNS.addService("_miot", "_tcp", 80);
        MDNS.addServiceTxt("_miot", "_tcp", "product", m_productName.c_str());
        MDNS.addServiceTxt("_miot", "_tcp", "deviceid", m_deviceId.c_str());
        MDNS.addServiceTxt("_miot", "_tcp", "version", String(m_version).c_str());

        MIOT_LOG("Deviceurl: http://%s.local\n", hostName.c_str());
    }
    else
    {
        MIOT_LOG("Error setting up MDNS responder\n");
    }
}


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
            if (!ssid.isEmpty())
            {
                String passphrase = m_preferences.getString("passphrase", "");
                MIOT_LOG("Found credentials, trying to connect to WiFi '%s'\n", ssid.c_str());

                // Yes, start connecting using these settings
                WiFi.begin(ssid.c_str(), passphrase.c_str());
                changeState(MIOTState_WaitingForWifi);
            }
            else
            {
                changeState(MIOTState_StartingSmartConfig);
            }
            m_preferences.end();
        }
        break;

    case MIOTState_StartingSmartConfig:
        WiFi.stopSmartConfig();
        if (WiFi.beginSmartConfig())
        {
            MIOT_LOG("Waiting for SmartConfig.\n");
            changeState(MIOTState_WaitingForSmartConfig);
        }
        break;

    case MIOTState_WaitingForSmartConfig:
        if (WiFi.smartConfigDone())
        {
            MIOT_LOG("SmartConfig credentials received. Waiting for WiFi.\n");
            changeState(MIOTState_WaitingForWifi); // Smart config succeeded, now trying to connect to Wifi
        }
        else
        {
            // When we do not receive smart config message, retry the stored credentials (if any)
            if ((millis() - m_millisLastStateChange) > m_smartConfigTimeout)
            {
                MIOT_LOG("SmartConfig timed-out. Retrying WiFi credentials\n");
                changeState(MIOTState_Unconnected);
            }
        }
        break;

    case MIOTState_WaitingForWifi:
        // Our we connected to Wifi yet?
        if (WiFi.status() == WL_CONNECTED)
        {
            MIOT_LOG("WiFi connected to %s\n", WiFi.SSID().c_str());
            changeState(MIOTState_Connected);

            // Start advertising our name
            advertise();

            // Store the WiFi credentials
            if (m_preferences.begin(MIOT_PREF_CONFIG, false)) // Open storage
            {
                m_preferences.putString("ssid", WiFi.SSID());
                m_preferences.putString("passphrase", WiFi.psk());
                m_preferences.end();
            }
        }
        else
        {
            // Check timeout
            if ((millis() - m_millisLastStateChange) > m_wifiConnectionTimeout)
            {
                MIOT_LOG("WiFi connection timeout. Restarting SmartConfig\n");
                changeState(MIOTState_StartingSmartConfig); // This takes too long, jump back to waiting for smartconfig
            }
        }
        break;

    case MIOTState_Connected:
        if (WiFi.status() != WL_CONNECTED)
        {
            MIOT_LOG("WiFi connection lost\n");
            changeState(MIOTState_ConnectionLost);
        }
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
                changeState(MIOTState_StartingSmartConfig);

                // Stop the MDNS
                MDNS.end();
            }
        }
        break;

    default: // Should never occur
        changeState(MIOTState_Unconnected);
        break;
    }
}
