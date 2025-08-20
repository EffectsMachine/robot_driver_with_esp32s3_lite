#include "espasyncbutton.hpp"

#define BUTTON_UP    GPIO_NUM_10
#define BUTTON_DOWN  GPIO_NUM_11
#define BUTTON_LEFT  GPIO_NUM_12
#define BUTTON_RIGHT GPIO_NUM_9
#define BUTTON_OK    GPIO_NUM_2

using ESPButton::event_t;

GPIOButton<ESPEventPolicy> bUp(BUTTON_UP, LOW);
GPIOButton<ESPEventPolicy> bDown(BUTTON_DOWN, LOW);
GPIOButton<ESPEventPolicy> bLeft(BUTTON_LEFT, LOW);
GPIOButton<ESPEventPolicy> bRight(BUTTON_RIGHT, LOW);
GPIOButton<ESPEventPolicy> bOK(BUTTON_OK, LOW);

ButtonCallbackMenu menu;

/*
--- --- --- Robot Driver with ESP32S3 Lite --- --- ---
T0 - Device Control -->
             |-L0--- Select Device Type: STS/SCS/HL/HM
             |-L1--- Change Baud Rate: 500k/1M/1.5M/2M/2.5M/3M/4M
             |-L2--- Encoder Steps: 1024/4096/65536
             |-L3--- Encoder Angle: 360/300/220
             |-L4--- Select ID: 254/1/2/3...253 
             |-L5--- Torque Lock: ON/OFF
             |-L6--- Set/Move Middle: ON/OFF
             |-L7--- Angle Ctrl: +/-, --> Middle
             |-L8--- Torque Ctrl: +/-
             |-L9--- Change ID: /1/2/3...253
             |-L10-- Set Next Device: -->OK
             |-L11-- Exit: OK
T1 - ESP-NOW Control -->
*/

// #define DEVICE_CTRL_NUM 11

// int menuType = 0; // 0: Device Control, 1: ESP-NOW Control
// int menuLevel = 0; // 0: Device Control, 1: ESP-NOW Control
// bool insideMainMenu = false; // true: inside main menu, false: outside main menu

// String deviceTypeStr[5] = {"<-STS->", "<-SCS->", "<-HL ->", "<-HM ->", "<-ACT->"};
// int deviceTypeIndex = 0;

// String baudRateStr[7] = {"<-500k->", "<- 1M ->", "<-1.5M->", "<- 2M ->", "<-2.5M->", "<- 3M ->", "<- 4M ->"};
// int baudRateIndex = 1;

// String encoderStepsStr[3] = {"<-1024 ->", "<-4096 ->", "<-65536->"};
// int encoderStepsIndex = 0;

// String encoderAngleStr[3] = {"<-360 ->", "<-300 ->", "<-220 ->"};
// int encoderAngleIndex = 0;

// int selectDeviceID = 1;
// bool torqueLockCtrl = true;
// bool setMiddleCtrl = false;
// float angleCtrl = 0.0;
// int torqueCtrl = 0;
// int changeDeviceID = 1;

// void changeMainMenu(event_t e, const EventMsg* m){
//     switch(e){
//         // Use click event to move cursor Up and Down (just print message)
//         case event_t::click :
//             // we catch all 'click' events and check which gpio triggered it
//             if (insideMainMenu) {
//                 return;
//             }
//             if (m->gpio == BUTTON_UP){
//                 menuType = 0;
//                 screenCtrl.clearDisplay();
//                 screenCtrl.displayText(">Devices", 0, 0, 2);
//                 screenCtrl.displayText(" ESP-NOW", 0, 16, 2);
//             } else if (m->gpio == BUTTON_DOWN){
//                 menuType = 1;
//                 screenCtrl.clearDisplay();
//                 screenCtrl.displayText(" Devices", 0, 0, 2);
//                 screenCtrl.displayText(">ESP-NOW", 0, 16, 2);
//             }
//             break;
//         case event_t::longPress :
//             // if (m->gpio == BUTTON_RIGHT){
//             //     insideMainMenu = true;
//             //     screenCtrl.clearDisplay();
//             //     if (menuType == 0){
//             //         menu.assign(BUTTON_UP, 0, deviceCtrl);
//             //         menu.assign(BUTTON_DOWN, 0, deviceCtrl);
//             //         menu.assign(BUTTON_LEFT, 0, deviceCtrl);
//             //         menu.assign(BUTTON_RIGHT, 0, deviceCtrl);
//             //         screenCtrl.displayText("DeviceType", 0, 0, 2);
//             //         screenCtrl.displayText(deviceTypeStr[deviceTypeIndex], 0, 16, 2);
//             //     } else if (menuType == 1){
//             //         screenCtrl.displayText("ESP-NOW", 0, 0, 2);
//             //         screenCtrl.displayText("INFO", 0, 16, 2);
//             //     }
                
//             // } else if (m->gpio == BUTTON_LEFT){
//             //     insideMainMenu = false;
//             //     menu.assign(BUTTON_UP, 0, changeMainMenu);
//             //     menu.assign(BUTTON_DOWN, 0, changeMainMenu);
//             //     screenCtrl.clearDisplay();
//             //     if (menuType == 0){
//             //         screenCtrl.displayText(">Devices", 0, 0, 2);
//             //         screenCtrl.displayText(" ESP-NOW", 0, 16, 2);
//             //     } else if (menuType == 1){
//             //         screenCtrl.displayText(" Devices", 0, 0, 2);
//             //         screenCtrl.displayText(">ESP-NOW", 0, 16, 2);
//             //     }
//             // }
//             break;
//     }
// };
void runMission(String missionName, int intervalTime, int loopTimes);
void buttonEventHandler(event_t e, const EventMsg* m){
    switch(e){
        // Use click event to move cursor Up and Down (just print message)
        case event_t::click :
            // we catch all 'click' events and check which gpio triggered it
            if (m->gpio == BUTTON_UP){
                buttonBuzzer();
                runMission("up", 0, 1);
            } else if (m->gpio == BUTTON_DOWN){
                buttonBuzzer();
                runMission("down", 0, 1);
            } else if (m->gpio == BUTTON_LEFT){
                buttonBuzzer();
                runMission("left", 0, 1);
            } else if (m->gpio == BUTTON_RIGHT){
                buttonBuzzer();
                runMission("right", 0, 1);
            }
            break;
    }
}



void menu_init_RD() {
    wifi_mode_t mode;
    esp_err_t err = esp_wifi_get_mode(&mode);

    if (err == ESP_ERR_WIFI_NOT_INIT) {
        Serial.println("WiFi not initialized. Creating default event loop...");
        Serial0.println("WiFi not initialized. Creating default event loop...");
        ESP_ERROR_CHECK(esp_event_loop_create_default());
    } else {
        Serial.println("WiFi already initialized. No need to create default event loop.");
        Serial0.println("WiFi already initialized. No need to create default event loop.");
    }

    ESP_ERROR_CHECK(esp_event_handler_instance_register(EBTN_EVENTS,
        ESP_EVENT_ANY_ID,
        // this lambda will simply translate loop events into btn_callback_t callback function
        [](void* handler_args, esp_event_base_t base, int32_t id, void* event_data){
            menu.handleEvent(ESPButton::int2event_t(id), reinterpret_cast<EventMsg*>(event_data));
        }, 
        NULL, NULL)
    );

    bUp.enableEvent(event_t::click);
    bDown.enableEvent(event_t::click);
    bLeft.enableEvent(event_t::click);
    bRight.enableEvent(event_t::click);
    bOK.enableEvent(event_t::click);

    menu.assign(BUTTON_UP, 0, buttonEventHandler);
    menu.assign(BUTTON_DOWN, 0, buttonEventHandler);
    menu.assign(BUTTON_LEFT, 0, buttonEventHandler);
    menu.assign(BUTTON_RIGHT, 0, buttonEventHandler);

    bUp.enable();
    bDown.enable();
    bLeft.enable();
    bRight.enable();
    bOK.enable();
}