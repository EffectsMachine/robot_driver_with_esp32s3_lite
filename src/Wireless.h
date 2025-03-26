#ifndef WIRELESS_H
#define WIRELESS_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>

typedef void (*JsonCommandCallback)(const JsonDocument& jsonCmdInput);

  

//   esp_now_peer_info_t peerInfo;


class Wireless{
    public:
        bool setAP(String ssid, String password, int wifiChannel);
        bool setSTA(String ssid, String password);
        bool setWifiMode(int mode, String ap_ssid, String ap_password, int wifiChannel, String sta_ssid, String sta_password);
        int getRSSI_AP();
        int getRSSI_STA();
        String getAPIP();
        String getSTAIP();

        void espnowInit(bool longRange);
        bool setEspNowMode(int mode);
        String macToString(uint8_t mac[6]);
        uint8_t* getMac();
        bool setEspNowMac(uint8_t* mac);
        bool sendEspNow(uint8_t* mac, String data);
        void setJsonCommandCallback(JsonCommandCallback callback);
        void addMacToPeer(uint8_t* mac);
        // bool OnDataRecv(const esp_now_peer_info_t *info, const unsigned char* incomingData, int len);
};

#endif