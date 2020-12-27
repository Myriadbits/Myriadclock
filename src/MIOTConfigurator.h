/*  MIOTConfigurator
    Myriadbits IOT Configurator
    
    Copyright (c) 2020 Jochem Bakker. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    @file	    MIOTConfigurator.h
    @author	    jochem@myriadbits.com
    @version	0.0.1
 	@date	    2020-11-1

    Dependencies: 
        Wifi.h
        Preferences


    The purpose of this class is to have a fast and stable deploy and discovery mechanism
    for ESP32's (and ESP8266). 
    - The initial deployment is done by creating a softAP with a fixed SSID and softAPPassword (configurable). 
    - When a connection is made by a client to this softAP, the softAP listens for a json packet with the WiFi credentials for
      the host WiFi network.
    - After receiving the credentials, this class will try to connect to the WiFi with the supplied credentials
    - Upon success, it will join a UDP multicast group and start broadcasting its information (productname, deviceid and version)
    - The WiFi credentials will be stored. Upon a reboot, those credentials will be re-used
    - When the WiFi connection is lost for some time a retry mechanism kicks in, after repeated failures, the initial
      deployment sequence is restarted.

    This class works in collaboration with the MiotConfigurator, an App (android/ios) to fully automate this process
*/

#pragma once

#include <ArduinoJson.h>
#include <Preferences.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#if defined(ARDUINO_ARCH_ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
    #include <WiFi.h>
#endif

#include <vector>

#include "MIOTConfigItem.h"
#include "MIOTMessage.h"


// TODO Remove
#define  MIOT_DEBUG

// Debug output destination can be defined externally with MIOT_DEBUG_PORT
#ifndef MIOT_DEBUG_PORT
    #define MIOT_DEBUG_PORT Serial
#endif 
#ifdef MIOT_DEBUG
    #define MIOT_LOG(...) do {MIOT_DEBUG_PORT.printf("[MIOT %lu] ", millis()); MIOT_DEBUG_PORT.printf( __VA_ARGS__ ); MIOT_DEBUG_PORT.println();} while (0)
#else
    #define MIOT_LOG(...)
#endif 

///////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////

#define MIOT_PREF_CONFIG                "MIOTConfig"  
#define MIOT_TIMEOUT_WIFICONNECTION     (30 * 1000)
#define MIOT_TIMEOUT_WIFICONNECTIONLOST (2 * 60 * 1000)
#define MIOT_TIMEOUT_SMARTCONFIG        (10 * 60 * 1000)
#define MIOT_DEFAULT_PRODUCTNAME        "MIOTDevice"
#define MIOT_DEFAULT_SOFTAPPASSWORD     "1234bier"
#define MIOT_DEFAULT_VERSION            1
#define MIOT_AP_SETTINGS_PORT           19785 // 4d 49 = "MI"        

#define MIOT_MULTICAST_IP4_ADDRESS      "232.10.11.12"
#define MIOT_MULTICAST_UDP_PORT         2323
#define MIOT_MULTICAST_TTL              32 // Restrict multicasts to same site

#define MIOT_SERVICE_UUID               "a8264447-fdbd-4668-a422-9c38494f5400"
#define MIOT_CHAR_INFO_UUID             "a8264447-fdbd-4668-a422-9c3d494f5401"
#define MIOT_CHAR_REQUEST_UUID          "a8264447-fdbd-4668-a422-9c3d494f5402"
#define MIOT_CHAR_RESPONSE_UUID         "a8264447-fdbd-4668-a422-9c3d494f5402"

// App requests config item X, MIOT response with 
// <command>
//      - 1: getConfigItem X
//           GetConfigResponse: 81 - X - value - name - description
//      - 2: setConfigItem X - Newvalue
//           SetConfigResponse: 82 - x - ok
//

typedef enum
{
    MIOTState_Unconnected,
    MIOTState_StartingAPSettings,
    MIOTState_WaitingForAPSettings,
    MIOTState_WaitingForWifi,
    MIOTState_Connected,
    MIOTState_ConnectionLost
} EMIOTState;


//
// Callbacks called by MIOT class
class MIOTCallbacks
{
public:
    virtual ~MIOTCallbacks() {};

	//
    // Callback that will be called with a random passkey to should be displayed
    //
	virtual void onDisplayPassKey(uint32_t passkey) = 0;

    //
    // Callback that will be called when the bluetooth connection is established or has failed
    //
	virtual void onBluetoothConnection(bool success) = 0;
};

///////////////////////////////////////////////////////////////////////////////
// MIOTConfigurator
// Automatic SmartConfig/Wifi connection
///////////////////////////////////////////////////////////////////////////////
class MIOTConfigurator : public BLESecurityCallbacks, public BLECharacteristicCallbacks
{
public:
    MIOTConfigurator(WiFiUDP& udp, std::string productName = MIOT_DEFAULT_PRODUCTNAME, int version = MIOT_DEFAULT_VERSION);

    void setup(MIOTCallbacks* pCallBacks, std::string softAPPassword = MIOT_DEFAULT_SOFTAPPASSWORD, unsigned long wifiConnectionTimeout = MIOT_TIMEOUT_WIFICONNECTION, unsigned long wifiConnectionLostTimeout = MIOT_TIMEOUT_WIFICONNECTIONLOST, unsigned long smartConfigTimeout = MIOT_TIMEOUT_SMARTCONFIG);    
    void handleClient();        

    void setProductName(std::string productName) { m_productName = productName; }
    void setDeviceId(std::string deviceId) { m_deviceId = deviceId; }
    void setVersion(int version) { m_version = version; }

    std::string getProductName() {return  m_productName; }
    std::string getDeviceId() { return m_deviceId; }
    int getVersion() { return m_version; }

    // Config items related
    void addConfigItem(uint32_t id, EConfigType type, std::string name, std::string synopsis);

	virtual uint32_t onPassKeyRequest();
	virtual void onPassKeyNotify(uint32_t pass_key);
	virtual bool onSecurityRequest();
	virtual void onAuthenticationComplete(esp_ble_auth_cmpl_t);
	virtual bool onConfirmPIN(uint32_t pin);

	virtual void onWrite(BLECharacteristic* pCharacteristic);

private:
    void changeState(EMIOTState newState);
    int createMulticastGroup();    
    bool handleMulticast(int sock);

protected:
    // Internal stuff
    MIOTCallbacks*          m_pCallBacks;
    BLEServer*              m_pBLEServer;

    // Config items
    std::vector<MIOTConfigItem>  m_configItems;

    // Statemachine related
    EMIOTState              m_state;
    unsigned long           m_wifiConnectionTimeout;        // Timeout for new wifi networks
    unsigned long           m_wifiConnectionLostTimeout;    // Timeout in ms when a Wifi connection is lost before starting the smartconfig again
    unsigned long           m_wifiAPSettingsTimeout;        // Timeout for the AP Settings, retry stored WiFi credentials
    unsigned long           m_millisLastStateChange;
    unsigned long           m_millisLastUDP;
    Preferences             m_preferences;

    BLECharacteristic       *m_pCharInfo;
    BLECharacteristic       *m_pCharRequest;
    BLECharacteristic       *m_pCharResponse;

    // Product info
    std::string             m_productName;
    std::string             m_softAPPassword;
    std::string             m_deviceId;
    std::string             m_deviceName;
    int                     m_version;
    WiFiUDP*                m_pUDP;
    int                     m_clients;

    int                     m_multicastSocket;
    WiFiServer              m_server;
protected:
};
