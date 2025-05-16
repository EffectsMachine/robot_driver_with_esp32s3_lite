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

#define DEVICE_CTRL_NUM 11

int menuType = 0; // 0: Device Control, 1: ESP-NOW Control
int menuLevel = 0; // 0: Device Control, 1: ESP-NOW Control
bool insideMainMenu = false; // true: inside main menu, false: outside main menu

String deviceTypeStr[5] = {"<-STS->", "<-SCS->", "<-HL ->", "<-HM ->", "<-ACT->"};
int deviceTypeIndex = 0;

String baudRateStr[7] = {"<-500k->", "<- 1M ->", "<-1.5M->", "<- 2M ->", "<-2.5M->", "<- 3M ->", "<- 4M ->"};
int baudRateIndex = 1;

String encoderStepsStr[3] = {"<-1024 ->", "<-4096 ->", "<-65536->"};
int encoderStepsIndex = 0;

String encoderAngleStr[3] = {"<-360 ->", "<-300 ->", "<-220 ->"};
int encoderAngleIndex = 0;

int selectDeviceID = 1;
bool torqueLockCtrl = true;
bool setMiddleCtrl = false;
float angleCtrl = 0.0;
int torqueCtrl = 0;
int changeDeviceID = 1;

void cycleIndex(int &A, int minVal, int maxVal, int step) {
    A += step;
    if (A > maxVal) {
        A = minVal;
    } else if (A < minVal) {
        A = maxVal;
    }
}

void deviceCtrl(event_t e, const EventMsg* m){
    if (!insideMainMenu || menuType != 0){
        return;
    }
    switch(e){
        // Use click event to move cursor Up and Down (just print message)
        case event_t::click :
            // we catch all 'click' events and check which gpio triggered it
            if (m->gpio == BUTTON_UP){
                cycleIndex(menuLevel, 0, DEVICE_CTRL_NUM, -1);
            } else if (m->gpio == BUTTON_DOWN){
                cycleIndex(menuLevel, 0, DEVICE_CTRL_NUM, 1);
            }
            screenCtrl.clearDisplay();
            switch(menuLevel){
                case 0:
                    screenCtrl.displayText("DeviceType", 0, 0, 2);
                    if (m->gpio == BUTTON_LEFT){
                        cycleIndex(deviceTypeIndex, 0, 4, -1);
                    } else if (m->gpio == BUTTON_RIGHT){
                        cycleIndex(deviceTypeIndex, 0, 4, 1);
                    }
                    screenCtrl.displayText(deviceTypeStr[deviceTypeIndex], 0, 16, 2);
                    break;
                case 1:
                    screenCtrl.displayText("BaudRate", 0, 0, 2);
                    if (m->gpio == BUTTON_LEFT){
                        cycleIndex(baudRateIndex, 0, 6, -1);
                    } else if (m->gpio == BUTTON_RIGHT){
                        cycleIndex(baudRateIndex, 0, 6, 1);
                    }
                    screenCtrl.displayText(baudRateStr[baudRateIndex], 0, 16, 2);
                    break;
                case 2:
                    screenCtrl.displayText("En-Steps", 0, 0, 2);
                    if (m->gpio == BUTTON_LEFT){
                        cycleIndex(encoderStepsIndex, 0, 2, -1);
                    } else if (m->gpio == BUTTON_RIGHT){
                        cycleIndex(encoderStepsIndex, 0, 2, 1);
                    }
                    screenCtrl.displayText(encoderStepsStr[encoderStepsIndex], 0, 16, 2);
                    break;
                case 3:
                    screenCtrl.displayText("En-Angle", 0, 0, 2);
                    if (m->gpio == BUTTON_LEFT){
                        cycleIndex(encoderAngleIndex, 0, 2, -1);
                    } else if (m->gpio == BUTTON_RIGHT){
                        cycleIndex(encoderAngleIndex, 0, 2, 1);
                    }
                    screenCtrl.displayText(encoderAngleStr[encoderAngleIndex], 0, 16, 2);
                    break;
                case 4:
                    screenCtrl.displayText("Select ID", 0, 0, 2);
                    if (m->gpio == BUTTON_LEFT){
                        if (selectDeviceID == 1){
                            selectDeviceID = 254;
                        } else {
                            cycleIndex(selectDeviceID, 1, 253, -1);
                        }
                    } else if (m->gpio == BUTTON_RIGHT){
                        cycleIndex(selectDeviceID, 1, 253, 1);
                    }
                    screenCtrl.displayText(String(selectDeviceID), 0, 16, 2);
                    break;
                case 5:
                    screenCtrl.displayText("TorqueLock", 0, 0, 2);
                    if (m->gpio == BUTTON_LEFT){
                        torqueLockCtrl = false;
                        screenCtrl.displayText("<- OFF ->", 0, 16, 2);
                    } else if (m->gpio == BUTTON_RIGHT){
                        torqueLockCtrl = true;
                        screenCtrl.displayText("<- ON  ->", 0, 16, 2);
                    }
                    break;
                case 6:
                    screenCtrl.displayText("Middle", 0, 0, 2);
                    break;
                case 7:
                    screenCtrl.displayText("AngleCtrl", 0, 0, 2);
                    break;
                case 8:
                    screenCtrl.displayText("TorqueCtrl", 0, 0, 2);
                    break;
                case 9:
                    screenCtrl.displayText("Change ID", 0, 0, 2);
                    break;
                case 10:
                    screenCtrl.displayText("Set Next", 0, 0, 2);
                    break;
                case 11:
                    screenCtrl.displayText("Exit", 0, 0, 2);
                    break;
                }
            break;
        case event_t::longPress :
            if (m->gpio == BUTTON_OK){
                Serial.println("OK Pressed");
            } else if (m->gpio == BUTTON_LEFT){
                Serial.println("Left Pressed");
            }
            break;
    }
};

void changeMainMenu(event_t e, const EventMsg* m){
    switch(e){
        // Use click event to move cursor Up and Down (just print message)
        case event_t::click :
            // we catch all 'click' events and check which gpio triggered it
            if (insideMainMenu) {
                return;
            }
            if (m->gpio == BUTTON_UP){
                menuType = 0;
                screenCtrl.clearDisplay();
                screenCtrl.displayText(">Devices", 0, 0, 2);
                screenCtrl.displayText(" ESP-NOW", 0, 16, 2);
            } else if (m->gpio == BUTTON_DOWN){
                menuType = 1;
                screenCtrl.clearDisplay();
                screenCtrl.displayText(" Devices", 0, 0, 2);
                screenCtrl.displayText(">ESP-NOW", 0, 16, 2);
            }
            break;
        case event_t::longPress :
            if (m->gpio == BUTTON_RIGHT){
                insideMainMenu = true;
                screenCtrl.clearDisplay();
                if (menuType == 0){
                    menu.assign(BUTTON_UP, 0, deviceCtrl);
                    menu.assign(BUTTON_DOWN, 0, deviceCtrl);
                    menu.assign(BUTTON_LEFT, 0, deviceCtrl);
                    menu.assign(BUTTON_RIGHT, 0, deviceCtrl);
                    screenCtrl.displayText("DeviceType", 0, 0, 2);
                    screenCtrl.displayText(deviceTypeStr[deviceTypeIndex], 0, 16, 2);
                } else if (menuType == 1){
                    screenCtrl.displayText("ESP-NOW", 0, 0, 2);
                    screenCtrl.displayText("INFO", 0, 16, 2);
                }
                
            } else if (m->gpio == BUTTON_LEFT){
                insideMainMenu = false;
                menu.assign(BUTTON_UP, 0, changeMainMenu);
                menu.assign(BUTTON_DOWN, 0, changeMainMenu);
                screenCtrl.clearDisplay();
                if (menuType == 0){
                    screenCtrl.displayText(">Devices", 0, 0, 2);
                    screenCtrl.displayText(" ESP-NOW", 0, 16, 2);
                } else if (menuType == 1){
                    screenCtrl.displayText(" Devices", 0, 0, 2);
                    screenCtrl.displayText(">ESP-NOW", 0, 16, 2);
                }
            }
            break;
    }
};


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

    bUp.enableEvent(event_t::press,      false);
    bUp.enableEvent(event_t::release,    false);
    bDown.enableEvent(event_t::press,    false);
    bDown.enableEvent(event_t::release,  false);
    bLeft.enableEvent(event_t::press,    false);
    bLeft.enableEvent(event_t::release,  false);
    bRight.enableEvent(event_t::press,   false);
    bRight.enableEvent(event_t::release, false);
    bOK.enableEvent(event_t::press,      false);
    bOK.enableEvent(event_t::release,    false);

    bUp.enableEvent(event_t::click);
    bDown.enableEvent(event_t::click);
    bLeft.enableEvent(event_t::click);
    bRight.enableEvent(event_t::click);
    bOK.enableEvent(event_t::click);

    bUp.enableEvent(event_t::longPress);
    bDown.enableEvent(event_t::longPress);
    bLeft.enableEvent(event_t::longPress);
    bRight.enableEvent(event_t::longPress);
    bOK.enableEvent(event_t::longPress);

    menu.assign(BUTTON_UP, 0, changeMainMenu);
    menu.assign(BUTTON_DOWN, 0, changeMainMenu);
    menu.assign(BUTTON_LEFT, 0, changeMainMenu);
    menu.assign(BUTTON_RIGHT, 0, changeMainMenu);

    bUp.enable();
    bDown.enable();
    bLeft.enable();
    bRight.enable();
    bOK.enable();
}