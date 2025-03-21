#ifndef WIRELESS_H
#define WIRELESS_H

#include <Arduino.h>

class Wireless{
    public:
        Wireless();
        bool setWifiMode(int mode);
        bool setAP(String ssid, String password);
        bool setSTA(String ssid, String password);
        String getWifiInfo();
        bool stopWifi();

};

#endif