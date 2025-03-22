#ifndef WIRELESS_H
#define WIRELESS_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>

class Wireless{
    public:
        bool setAP(String ssid, String password, int wifiChannel);
        bool setSTA(String ssid, String password);
        bool setWifiMode(int mode, String ap_ssid, String ap_password, int wifiChannel, String sta_ssid, String sta_password);
        int getRSSI_AP();
        int getRSSI_STA();
        String getAPIP();
        String getSTAIP();

        bool setEspNowMode(int mode);
        uint8_t* getMac();
        String macToString(uint8_t* mac);
        bool setEspNowMac(uint8_t* mac);
        bool sendEspNow(uint8_t* mac, String data);
        typedef void (*JsonCommandCallback)(const JsonDocument& jsonCmdInput);
        JsonCommandCallback jsonCommandCallback = nullptr;
        void setJsonCommandCallback(JsonCommandCallback callback);
        void OnDataRecv(const esp_now_peer_info_t *info, const unsigned char* incomingData, int len);
};

#endif