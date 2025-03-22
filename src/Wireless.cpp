#include "Wireless.h"

#define WIFI_MODE_NONE 0
#define WIFI_MODE_AP_STA 1

int wifiMode = 1;
int maxClients = 1;
bool statusAP = false;
bool statusSTA = false;

bool Wireless::setAP(String ssid, String password, int wifiChannel) {
    if (wifiMode == WIFI_MODE_NONE) {
        Serial.println("WiFi mode is None, skip configuring Access Point");
        Serial0.println("WiFi mode is None, skip configuring Access Point");
        return false;
    }
    if (ssid.length() == 0) {
        Serial.println("SSID is empty, skip configuring Access Point");
        Serial0.println("SSID is empty, skip configuring Access Point");
        return false;
    }
    // WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssid.c_str(), password.c_str(), wifiChannel, 0, maxClients);
    if (WiFi.softAPIP()) {
        Serial.println("Access Point started");
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP());
        Serial0.println("Access Point started");
        Serial0.print("IP Address: ");
        Serial0.println(WiFi.softAPIP());
        return true;
    } else {
        Serial.println("Failed to start Access Point");
        Serial0.println("Failed to start Access Point");
        return false;
    }
}

bool Wireless::setSTA(String ssid, String password) {
    if (wifiMode == WIFI_MODE_NONE) {
        Serial.println("WiFi mode is None, skip configuring Station");
        Serial0.println("WiFi mode is None, skip configuring Station");
        return false;
    }
    if (ssid.length() == 0) {
        Serial.println("SSID is empty, skip configuring Station");
        Serial0.println("SSID is empty, skip configuring Station");
        return false;
    }
    // WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to WiFi");
    Serial0.print("Connecting to WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        Serial0.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial0.println("\nWiFi connected");
        Serial0.print("IP Address: ");
        Serial0.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("\nFailed to connect to WiFi");
        Serial0.println("\nFailed to connect to WiFi");
        return false;
    }
}

bool Wireless::setWifiMode(int mode, String ap_ssid, String ap_password, int wifiChannel, String sta_ssid, String sta_password) {
    if(mode == WIFI_MODE_NONE) {
        WiFi.mode(WIFI_OFF);
        wifiMode = WIFI_MODE_NONE;
        return false;
    } else if (mode == WIFI_MODE_AP_STA) {
        WiFi.mode(WIFI_AP_STA);
        wifiMode = WIFI_MODE_AP_STA;
        setAP(ap_ssid.c_str(), ap_password.c_str(), wifiChannel);
        if(setSTA(sta_ssid.c_str(), sta_password.c_str())) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

int Wireless::getRSSI_STA() {
    if (WiFi.status() == WL_CONNECTED) {
        int rssi = WiFi.RSSI();
        return rssi;
    } else {
        return 1;
    }
}

int Wireless::getRSSI_AP() {
    wifi_sta_list_t stationList;
    esp_wifi_ap_get_sta_list(&stationList);
    wifi_sta_info_t station = stationList.sta[0];
    return station.rssi;
}

String Wireless::getAPIP() {
    if (WiFi.softAPIP()) {
        return WiFi.softAPIP().toString();
    } else {
        return "";
    }
}

String Wireless::getSTAIP() {
    if (WiFi.localIP()) {
        return WiFi.localIP().toString();
    } else {
        return "";
    }
}