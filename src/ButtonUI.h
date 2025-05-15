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

// void insideTypeCtrl(event_t e, const EventMsg* m){
//     switch(e){
//         // Use click event to move cursor Up and Down (just print message)
//         case event_t::click :
//             // we catch all 'click' events and check which gpio triggered it
//             // BUTTON_1 will move cursor "Up", BUTTON_2 will move it "down"
//             Serial.print("Cursor "); Serial.println(m->gpio == BUTTON_UP ? "Up" : "Down");
//             break;
//         // any button's longpress event will cycle menu level 0->1->2->3->0
//         case event_t::longPress :
//             insideMenuLevelToggle();
//             break;
//     }
// };

// void cursor_control(event_t e, const EventMsg* m){
//     switch(e){
//         // Use click event to move cursor Up and Down (just print message)
//         case event_t::click :
//             // we catch all 'click' events and check which gpio triggered it
//             // BUTTON_1 will move cursor "Up", BUTTON_2 will move it "down"
//             Serial.print("Cursor "); Serial.println(m->gpio == BUTTON_UP ? "Up" : "Down");
//             break;
//         // any button's longpress event will cycle menu level 0->1->2->3->0
//         case event_t::longPress :
//             menu_toggle();
//             break;
//     }
// };

// void volume_control(event_t e, const EventMsg* m){
//     switch(e){
//         // Use click event to control Volume Up and Down (just print)
//         case event_t::click :
//             Serial.print("Volume "); Serial.println(m->gpio == BUTTON_UP ? "Up" : "Down");
//             break;
//         // any button longpress event will cycle menu level 0->1->2->0
//         case event_t::longPress :
//             menu_toggle();
//             break;
//     }
// };

// void counters(event_t e, const EventMsg* m){
//     switch(e){
//     // on a single click we will instruct user to do something more with buttons
//     case event_t::click :
//         if (m->gpio == BUTTON_UP)
//             Serial.println("Hold button 1 for autorepeat...");
//         else
//             Serial.println("Single click is not that fun, try double or triple clicks...");
//         break;
//     // autorepeat action
//     case event_t::autoRepeat :
//         // I'll print "*" on each autorepeat event for button_1 and '#' for button_2
//         if (m->gpio == BUTTON_UP)
//             Serial.print("*");
//         else
//             Serial.print("#");
//         break;
//     // multiclicks
//     case event_t::multiClick :
//         Serial.printf("gpio: %d clicked %u times. ", m->gpio, m->cntr);
//         Serial.println("Click 4 times to enable autorepeat for Button 2");
//         Serial.println("Click 5 times to exit to menu level 0");
//         // toggle menu on 5th click
//         if (m->cntr == 4){
//             bDown.enableEvent(event_t::autoRepeat);
//             Serial.println("Button 2 autorepeats enabled, try it");
//         }

//         if (m->cntr == 5)
//             menu_toggle();
//         break;
//     }

//     /**
//      * since we are using autorepeat above for astesk printing, we can no longer use 'longPress' event to toggle menu,
//      * because 'autorepeat' is triggered after 'longPress' event
//        so, let's ignore it here
//     */
//     //case event_t::longPress :
//     //    menu_toggle();
//     //    break;
// };

// void insideMenuLevelToggle(int8_t changeLevel) {
//     // cycle switch menu levels 0->1->2->0 ...
//     menu.setMenuLevel( (menu.getMenuLevel()+changeLevel)%11 );

//     // on menu level 2 we will enable autorepeat for  button_1 and multiclick counter for button_2
//     if (menu.getMenuLevel() == 2){
//         bUp.enableEvent(event_t::autoRepeat);
//         bDown.enableEvent(event_t::multiClick);
//     } else {
//         // on other levels but 2 we disable it
//         bUp.enableEvent(event_t::autoRepeat, false);
//         bDown.enableEvent(event_t::multiClick, false);
//     }
//     Serial.print("Switched to menu level:"); Serial.println(menu.getMenuLevel());
// };

void menu_init_RD() {
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

    // menu.assign(BUTTON_UP, 0, insideTypeCtrl);
    // menu.assign(BUTTON_DOWN, 0, insideTypeCtrl);

    // menu.assign(BUTTON_UP, 1, volume_control);
    // menu.assign(BUTTON_DOWN, 1, volume_control);

    // menu.assign(BUTTON_UP, 2, counters);
    // menu.assign(BUTTON_DOWN, 2, counters);

    // menu.assign(BUTTON_OK, 0, okButtonCallbackMainMenu);

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