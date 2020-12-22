#include <string.h>
#include <sys/param.h>

#include "MIOTConfigurator.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

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
// setup the MIOT Configurator, call this method in the setup function
// This setup will:
// - Start WiFi using the stored credentials (if any and if valid)
// - If no credentials are found, a SoftAP is started with a unique deviceID as SSID
//   and the password (as argument)
// Note that the password should be entered by the user of the configuration tool
// Might be good to add that as a QR code to the product itself
void MIOTConfigurator::setup(String softAPPassword, unsigned long wifiConnectionTimeout, unsigned long wifiConnectionLostTimeout, unsigned long wifiAPSettingTimeout)
{
    m_softAPPassword = softAPPassword;
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
// Create a multicast group (if it does not exist yet)
// Add a socket, either IPV4-only or IPV6 dual mode, to the IPV4 multicast group
int MIOTConfigurator::createMulticastGroup()
{
    struct sockaddr_in saddr = {0};
    int sock = -1;
    int err = 0;

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0)
    {
        MIOT_LOG("Failed to create socket. Error %d", errno);
        return -1;
    }

    // Bind the socket to any address
    saddr.sin_family = PF_INET;
    saddr.sin_port = htons(MIOT_MULTICAST_UDP_PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (err < 0)
    {
        MIOT_LOG("Failed to bind socket. Error %d", errno);
        close(sock);
        return -1;
    }

    // Assign multicast TTL (set separately from normal interface TTL)
    uint8_t ttl = MIOT_MULTICAST_TTL;
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(uint8_t));
    if (err < 0)
    {
        MIOT_LOG("Failed to set IP_MULTICAST_TTL. Error %d", errno);
        close(sock);
        return -1;
    }

    // All set, socket is configured for sending and receiving
    return sock;
}

//
// Handle the multicasts
bool MIOTConfigurator::handleMulticast(int sock)
{
    struct timeval tv = 
    {
        .tv_sec = 2,
        .tv_usec = 0,
    };
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sock, &rfds);

    int s = select(sock + 1, &rfds, NULL, NULL, &tv);
    if (s < 0)
    {
        MIOT_LOG("Select failed: errno %d", errno);
        return false;
    }
    else if (s > 0)
    {
        if (FD_ISSET(sock, &rfds))
        {
            // Incoming datagram received
            char recvbuf[48];
            char raddr_name[32] = {0};

            struct sockaddr_in6 raddr; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(raddr);
            int len = recvfrom(sock, recvbuf, sizeof(recvbuf) - 1, 0, (struct sockaddr *)&raddr, &socklen);
            if (len < 0)
            {
                MIOT_LOG("multicast recvfrom failed: errno %d", errno);
                return false;
            }

            // Get the sender's address as a string
            if (raddr.sin6_family == PF_INET)
            {
                inet_ntoa_r(((struct sockaddr_in *)&raddr)->sin_addr.s_addr, raddr_name, sizeof(raddr_name) - 1);
            }
            MIOT_LOG("received %d bytes from %s:", len, raddr_name);

            recvbuf[len] = 0; // Null-terminate whatever we received and treat like a string...
            MIOT_LOG("%s", recvbuf);
        }
    }
    else
    { 
        // s == 0
        // Timeout passed with no incoming data, so send something!
        char sendbuf[256];
        char addrbuf[32] = {0};
        int len = snprintf(sendbuf, sizeof(sendbuf), "{\"type\":\"%s\",\"id\":\"%s\",\"name\":\"%s\",\"alive\":\"%ld\"}", 
            m_productName.c_str(), m_deviceId.c_str(), m_deviceName.c_str(), millis() / 1000 );
        if (len > sizeof(sendbuf))
        {
            MIOT_LOG("Overflowed multicast sendfmt buffer!!");
            return false;
        }

        struct addrinfo hints = {0};
        hints.ai_flags = AI_PASSIVE;
        hints.ai_socktype = SOCK_DGRAM;
        struct addrinfo *res;

        hints.ai_family = AF_INET; // For an IPv4 socket
        int err = getaddrinfo(MIOT_MULTICAST_IP4_ADDRESS, NULL, &hints, &res);
        if (err < 0)
        {
            MIOT_LOG("getaddrinfo() failed for IPV4 destination address. error: %d", err);
            return false;
        }

        ((struct sockaddr_in *)res->ai_addr)->sin_port = htons(MIOT_MULTICAST_UDP_PORT);
        inet_ntoa_r(((struct sockaddr_in *)res->ai_addr)->sin_addr, addrbuf, sizeof(addrbuf) - 1);
        MIOT_LOG("Sending to IPV4 multicast address %s [%s]...\n", addrbuf, sendbuf);
        err = sendto(sock, sendbuf, len, 0, res->ai_addr, res->ai_addrlen);
        if (err < 0)
        {
            MIOT_LOG("IPV4 sendto failed. errno: %d", errno);
            return false;
        }
    }
    return true;
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
            //if (false) 
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
        //WiFi.softAPsetHostname(m_deviceName.c_str());
        if (WiFi.softAP(m_deviceName.c_str(), m_softAPPassword.c_str(), 1))
        {                
            delay(100);
            if (WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0)))
            {
                MIOT_LOG("Start listening for SoftAP Setting\n");
                MIOT_LOG("- SoftAP IP Address:  %s\n", WiFi.softAPIP().toString().c_str());

                if (m_pUDP)
                {
                    if (!m_pUDP->begin(MIOT_AP_SETTINGS_PORT))
                    {
                        MIOT_LOG("- Failed to open UDP port!\n");
                        WiFi.softAPdisconnect();
                    }
                    else
                    {
                        MIOT_LOG("- SoftAP IP Port:  %d\n", MIOT_AP_SETTINGS_PORT);
                        MIOT_LOG("Waiting for SoftAP settings.\n");                  
                        changeState(MIOTState_WaitingForAPSettings);
                    }                    
                }
                else
                {
                    // Hmm, don't have udp, so try to connect to wifi again??
                    // Actually I don't know what to do now :(
                    changeState(MIOTState_WaitingForWifi);
                }
            }
        }
        else
        {
            MIOT_LOG("Failed to create the soft AP!\n");
        }
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

            int dataSize = m_pUDP->parsePacket();
            if (dataSize > 0) 
            {
                MIOT_LOG("Received %d bytes from %s, port %d\n", dataSize, m_pUDP->remoteIP().toString().c_str(), m_pUDP->remotePort());

                // Read the packet into packetBufffer
                char packetBuffer[2048]; //buffer to hold incoming packet,
                m_pUDP->read(packetBuffer, 2048);
                packetBuffer[dataSize] = 0;
                MIOT_LOG("-------> Received: %s.\n", packetBuffer);

                // Expected json format
                // {"ssid":"<ssid>", "passphrase":"*****"}
                StaticJsonDocument<2048> doc;
                if (deserializeJson(doc, packetBuffer) == DeserializationError::Ok)
                { 
                    String ssid = doc["ssid"];
                    String passphrase = doc["passphrase"];

                    // Send an ack
                    sprintf(packetBuffer, "ACK");
                    m_pUDP->beginPacket(m_pUDP->remoteIP(), m_pUDP->remotePort());
                    m_pUDP->write((const uint8_t*) packetBuffer, strlen(packetBuffer));
                    m_pUDP->endPacket();
                    MIOT_LOG("-------> Send: %s.\n", packetBuffer);

                    // Stop the UDP
                    m_pUDP->stop();

                    // Disconnect soft AP
                    //WiFi.softAPdisconnect();

                    // Yes, start connecting using these settings
                    WiFi.begin(ssid.c_str(), passphrase.c_str());

                    // Wait for the WiFi
                    changeState(MIOTState_WaitingForWifi);
                    MIOT_LOG("SmartConfig credentials received (%s, %s). Waiting for WiFi.\n", ssid.c_str(), passphrase.c_str());
                }
                else
                {
                    sprintf(packetBuffer, "NACK");
                    m_pUDP->beginPacket(m_pUDP->remoteIP(), m_pUDP->remotePort());
                    m_pUDP->write((const uint8_t*) packetBuffer, strlen(packetBuffer));
                    m_pUDP->endPacket();
                    MIOT_LOG("-------> Send: %s.\n", packetBuffer);
                }
            }
        }
        break;

    case MIOTState_WaitingForWifi:
        // Our we connected to Wifi yet?
        if (WiFi.status() == WL_CONNECTED)
        {
            // Yes, WiFi is connected!
            MIOT_LOG("WiFi connected to %s\n", WiFi.SSID().c_str());
            MIOT_LOG("- IP Address:  %s\n", WiFi.localIP().toString().c_str());
            MIOT_LOG("- Subnet mask: %s\n", WiFi.subnetMask().toString().c_str());
            MIOT_LOG("- Gateway:     %s\n", WiFi.gatewayIP().toString().c_str());

            // Store the WiFi credentials
            if (m_preferences.begin(MIOT_PREF_CONFIG, false)) // Open storage
            {
                m_preferences.putString("ssid", WiFi.SSID());
                m_preferences.putString("passphrase", WiFi.psk());
                m_preferences.end();
            } 

            // Now start registering ourself with the multicast group so we can be found
            m_multicastSocket = createMulticastGroup();
            if (m_multicastSocket >= 0)
            {
                // set destination multicast addresses for sending from these sockets
                struct sockaddr_in sdestv4 = {0};
                sdestv4.sin_family = PF_INET;
                sdestv4.sin_port = htons(MIOT_MULTICAST_UDP_PORT);

                // We know this inet_aton will pass because we did it above already
                inet_aton(MIOT_MULTICAST_IP4_ADDRESS, &sdestv4.sin_addr.s_addr);
            }

            changeState(MIOTState_Connected);
        }
        else
        {
            // Check timeout
            if ((millis() - m_millisLastStateChange) > m_wifiConnectionTimeout)
            {
                MIOT_LOG("WiFi connection timeout. Restarting SmartConfig\n");
                WiFi.disconnect();
                changeState(MIOTState_StartingAPSettings); // This takes too long, jump back to waiting for smartconfig
            }
        }
        break;

    case MIOTState_Connected:
        // Connected state, everything is working
        if (WiFi.status() != WL_CONNECTED)
        {
            MIOT_LOG("WiFi connection lost\n");
            changeState(MIOTState_ConnectionLost);
        }
        else
        {
            // Handle the multicast input
            //if (m_multicastSocket >= 0)
            //    handleMulticast(m_multicastSocket);
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
                changeState(MIOTState_StartingAPSettings);
            }
        }
        break;

    default: // Should never occur
        changeState(MIOTState_Unconnected);
        break;
    }
}
