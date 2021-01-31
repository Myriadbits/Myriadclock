///////////////////////////////////////////////////////////////////////////////
// MyriadclockSettings class
// Class for storing app wide settings
///////////////////////////////////////////////////////////////////////////////

#include "..\include\BLEConfig.h"
#include "..\include\BLEConfigItemWiFi.h"
#include <stdio.h> 
#include <algorithm>
#include <time.h> // time_t, struct tm, time, mktime

//
// Decode an incoming config item
// data: string containing the data
void BLEConfigItemWiFi::onDecodeData(std::string data)
{
    // The first byte contains the index of the WiFi option
    m_value = data[0]; // Store the index
    // And get the matching SSID
    MIOTConfigOption_t* poption = getOption(m_value);
    if (poption != NULL)
        m_sSSID = poption->m_sName.c_str();

    // Next bytes contain the passphrase
    if (data.length() > 1)
        m_sPassphrase = data.substr(1); 
}

//
// Load this config item from the preferences
void BLEConfigItemWiFi::onLoad(Preferences &preferences, char* pkey)
{
    m_sSSID = std::string(preferences.getString(BLECONFIG_PREFERENCES_SSID, "").c_str());
    m_sPassphrase = std::string(preferences.getString(BLECONFIG_PREFERENCES_PASSPHRASE, "").c_str());         

    // Load the X(10) strongest network names as options
    addWiFiSSIDOptions();

    // Select the new WiFi network
    WiFi.begin(m_sSSID.c_str(), m_sPassphrase.c_str());

    // Now select the sSSID if found
    for (auto it : m_vecOptions)
    {
        if (it.m_sName == m_sSSID)
        {
            m_value = it.m_value;
            break;
        }
    }

     BLECONFIG_LOG("Loading WiFi settings: '%s' (index %d) with passphrase: %s", m_sSSID.c_str(), m_value, m_sPassphrase.c_str());
}

//
// Store this config item to the preferences
void BLEConfigItemWiFi::onStore(Preferences &preferences, char* pkey)
{
    preferences.putString(BLECONFIG_PREFERENCES_SSID, m_sSSID.c_str());
    preferences.putString(BLECONFIG_PREFERENCES_PASSPHRASE, m_sPassphrase.c_str());

    // Special case, we do the WiFi ourself
    BLECONFIG_LOG("Setting WiFi to index %d", getValue());
    MIOTConfigOption_t* poption = getOption((uint8_t) getValue());
    if (poption != NULL)
    {
        BLECONFIG_LOG("Switching to network: '%s' with passphrase '%s'", poption->m_sName.c_str(), m_sPassphrase.c_str());
        m_sSSID = poption->m_sName;
        
        // Select the new WiFi network
        WiFi.begin(m_sSSID.c_str(), m_sPassphrase.c_str());
    }
}

//
// Fill all the SSIDs
void BLEConfigItemWiFi::addWiFiSSIDOptions()
{
    BLECONFIG_LOG("Scanning for WiFi networks...");
    int numSsid = WiFi.scanNetworks();
    if (numSsid == -1) 
    {
        BLECONFIG_LOG("Couldn't get a wifi connection!");
        return;        
    }

    uint8_t indices[numSsid];
    for (int i = 0; i< numSsid; i++)
        indices[i] = i;

    // Now do a simple sort 
    for (int i = 0; i < numSsid; i++) 
    {
        for (int j = i + 1; j < numSsid; j++) 
        {
            if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) 
            {
                std::swap(indices[i], indices[j]);  
            }
        }
    }

    BLECONFIG_LOG("Found %d WiFi networks", numSsid);
    clearOptions();
    int selected = 0;
    for (int i = 0; i < numSsid && i < 16; i++)  // Add a maximum of 16 networks!
    {
        if (WiFi.SSID(indices[i]) == WiFi.SSID())
            selected = i; // Remember the active SSID
        BLECONFIG_LOG("- Adding WiFi SSID '%s' [%d]", WiFi.SSID(indices[i]).c_str(), WiFi.RSSI(indices[i]));
        addOption(indices[i], WiFi.SSID(indices[i]).c_str());
    }
    setValue(selected);
}

//
// Return the value as a hex string (for debugging)
std::string BLEConfigItemWiFi::valueToString()
{
    //char stemp[32];
    //snprintf(stemp, 32, "%08X", m_value);
    return m_sSSID;
}
