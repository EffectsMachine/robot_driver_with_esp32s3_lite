// for PlatformIO. This is the main file of the project.
// This file is the entry point of the program.
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <nvs_flash.h>
#include <MuiPlusPlus.hpp>
#include "sbus.h"
#include "USB.h"
#include "USBCDC.h"
#include "tusb.h"

// #include "esp_heap_caps.h"

#include "Config.h"
// #include "RGBLight.h"
// #include "BodyCtrl.h"
#include "jointsCtrl.h"
#include "FilesCtrl.h"
#include "ScreenCtrl.h"
#include "Wireless.h"
#include "button.h"

JsonDocument jsonCmdReceive;
JsonDocument jsonFeedback;
USBCDC USBSerial; // Declare USBSerial as an instance of USBCDC
DeserializationError err;
String outputString;
// RGBLight led;
// BodyCtrl bodyCtrl;
JointsCtrl jointsCtrl;
FilesCtrl filesCtrl;
ScreenCtrl screenCtrl;
Wireless wireless;
// MuiPlusPlus menu;
bool newCmdReceived = false;
bool breakloop = false;
unsigned long tuneStartTime;
int* jointFeedback;

int jointsZeroPos[12];
int jointsCurrentPos[12];
void jsonCmdReceiveHandler(const JsonDocument& jsonCmdInput);
void runMission(String missionName, int intervalTime, int loopTimes);








void buttonBuzzer() {
  tone(BUZZER_PIN, 2000);
  delay(5);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(5);
  tone(BUZZER_PIN, 1000);
  delay(5);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(5);
  tone(BUZZER_PIN, 3000);
  delay(5);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, HIGH);
}


// S.BUS
// bfs::SbusRx sbus(&Serial0, 44, 43, true);
// bfs::SbusData sbusData;


void setup() {
  delay(200);
  Serial0.begin(BAUD_RATE);
  // S.BUS
  // sbus.Begin();
  Wire.begin(IIC_SDA, IIC_SCL);
  Wire.setClock(400000);
  Serial.println("device starting...");
  Serial0.println("device starting...");

  // Serial1.begin(1000000, SERIAL_8N1, 5, 4);
  // gqdmd.begin(&Serial1);
  // gqdmd.setTxEnd_T32(1000000);
  // gqdmd.setTimeOut(100);
  // delay(200);

  // buzzer
  // pinMode(BUZZER_PIN, OUTPUT);
  // buttonBuzzer();

  // fake args, it will be ignored by the USB stack, default baudrate is 12Mbps
  USBSerial.begin(BAUD_RATE);
  USB.begin();
  USBSerial.println("ESP32-S3 USB CDC DONE!");
  Serial0.println("ESP32-S3 USB CDC DONE!");

  // led.init();
  filesCtrl.init();

  jointsCtrl.init(1000000);
  jointsCtrl.setJointType(JOINT_TYPE_SC);
  jointsCtrl.setEncoderStepRange(1024, 220);
  delay(1000);
  jointsCtrl.moveMiddle(254);

  screenCtrl.init();
  screenCtrl.displayText("LYgion", 0, 0, 2);
  screenCtrl.displayText("Robotics", 0, 16, 2);



  if(!filesCtrl.checkMission("boot")) {
    filesCtrl.createMission("boot", "this is the boot mission.");
    filesCtrl.appendStep("boot", "{\"T\":400,\"mode\":1,\"ap_ssid\":\"LYgion\",\"ap_password\":\"12345678\",\"channel\":1,\"sta_ssid\":\"\",\"sta_password\":\"\"}");
  } 
  runMission("boot", 0, 1);

  // esp-now init
  // wireless.espnowInit(false);
  wireless.espnowInit(true);
  wireless.setJsonCommandCallback(jsonCmdReceiveHandler);

  // wireless.addMacToPeer(broadcastAddress);  
  // jsonFeedback.clear();
  // jsonFeedback["T"] = CMD_DISPLAY_SINGLE;
  // jsonFeedback["line"] = 1;
  // jsonFeedback["text"] = "Lygion Robotics";
  // jsonFeedback["update"] = 1;
  // wireless.sendEspNowJson(broadcastAddress, jsonFeedback);

  // jsonFeedback.clear();
  // jsonFeedback["T"] = CMD_DISPLAY_SINGLE;
  // jsonFeedback["line"] = 3;
  // jsonFeedback["text"] = "Lygion.ai";
  // jsonFeedback["update"] = 1;
  // wireless.sendEspNowJson(broadcastAddress, jsonFeedback);

  // disable simple press/release events, we do not need them
  // buttonUp.enableEvent(event_t::press,      false);
  // buttonUp.enableEvent(event_t::release,    false);
  // buttonDown.enableEvent(event_t::press,      false);
  // buttonDown.enableEvent(event_t::release,    false);
  // buttonLeft.enableEvent(event_t::press,      false);
  // buttonLeft.enableEvent(event_t::release,    false);
  // buttonRight.enableEvent(event_t::press,      false);
  // buttonRight.enableEvent(event_t::release,    false);
  // buttonOk.enableEvent(event_t::press,      false);
  // buttonOk.enableEvent(event_t::release,    false);

  // // enable clicks
  // buttonUp.enableEvent(event_t::click);
  // buttonDown.enableEvent(event_t::click);
  // buttonLeft.enableEvent(event_t::click);
  // buttonRight.enableEvent(event_t::click);
  // buttonOk.enableEvent(event_t::click);

  // // enable longpress to cycle through menu
  // buttonLeft.enableEvent(event_t::longPress);
  // buttonRight.enableEvent(event_t::longPress);

  buttonUp.begin();
  buttonUp.onPress([]() {
    Serial0.println("Button Up Pressed");
    screenCtrl.changeSingleLine(1, "Button Up Pressed", true);
  });
  buttonUp.onRelease([]() {
    Serial0.println("Button Up Released");
    screenCtrl.changeSingleLine(1, "Button Up Released", true);
  });
  buttonUp.enable();
}



bool runStep(String missionName, int step) {
  outputString = filesCtrl.readStep(missionName, step);
  err = deserializeJson(jsonCmdReceive, outputString);
  if (err == DeserializationError::Ok) {
    jsonCmdReceiveHandler(jsonCmdReceive);
    return true;
  } else {
    Serial.print("JSON parsing error (this is a normal output when booting or running Mission): ");
    Serial.println(err.c_str());
    return false;
  }
}



void runMission(String missionName, int intervalTime, int loopTimes) {
  intervalTime = intervalTime - timeOffset;
  if (intervalTime < 0) {intervalTime = 0;}
  int j = 1;
  while (true) {
    Serial.print("Running loop: ");
    Serial.println(j);
    int i = 1;
    while (true) {
      if (Serial.available() > 0) {
        break;
      }
      if (runStep(missionName, i)) {
        Serial.print("Step: ");
        Serial.println(i);
        i++;
        delay(intervalTime);
      } else {
        Serial.println("Mission Completed.");
        break;
      }
    }
    j++;
    if (j > loopTimes && j != -1) {
      break;
    }
  }
}



void jsonCmdReceiveHandler(const JsonDocument& jsonCmdInput){
  int cmdType;
  breakloop = true;
  cmdType = jsonCmdInput["T"].as<int>();
  switch(cmdType){
  // joints ctrl
  case CMD_SET_JOINTS_BAUD:
                        jointsCtrl.setBaudRate(jsonCmdInput["baud"]);
                        break;
  case CMD_SET_JOINTS_TYPE:
                        if (jointsCtrl.setJointType(jsonCmdInput["type"])) {
                          Serial0.println("Joint type set successfully.");
                          Serial.println("Joint type set successfully.");
                        } else {
                          Serial0.println("Failed to set joint type.");
                          Serial.println("Failed to set joint type.");
                        }
                        break;
  case CMD_SET_ENCODER:
                        if (jointsCtrl.setEncoderStepRange(jsonCmdInput["steps"], jsonCmdInput["angle"])) {
                          Serial0.println("Encoder step range set successfully.");
                          Serial.println("Encoder step range set successfully.");
                        } else {
                          Serial0.println("Failed to set encoder step range.");
                          Serial.println("Encoder step range set successfully.");
                        }
                        break;
  case CMD_SINGLE_FEEDBACK:
                        jointFeedback = jointsCtrl.singleFeedBack(jsonCmdInput["id"]);
                        jsonFeedback.clear();
                        jsonFeedback["T"] = -CMD_SINGLE_FEEDBACK;
                        if (jointFeedback[0] == -1) {
                          jsonFeedback["ps"] = -1;
                          serializeJson(jsonFeedback, outputString);
                          Serial.println(outputString);
                          Serial0.println(outputString);
                        } else {
                          jsonFeedback["ps"] = jointFeedback[0];
                          jsonFeedback["pos"] = jointFeedback[1];
                          jsonFeedback["spd"] = jointFeedback[2];
                          jsonFeedback["load"] = jointFeedback[3];
                          jsonFeedback["vol"] = jointFeedback[4];
                          jsonFeedback["temp"] = jointFeedback[5];
                          jsonFeedback["mov"] = jointFeedback[6];
                          jsonFeedback["curt"] = jointFeedback[7];
                          serializeJson(jsonFeedback, outputString);
                          Serial.println(outputString);
                          Serial0.println(outputString);
                        }
                        break;
  case CMD_PING:
                        jsonFeedback.clear();
                        jsonFeedback["T"] = -CMD_PING;
                        if (jointsCtrl.ping(jsonCmdInput["id"])) {
                          jsonFeedback["ps"] = 1;
                        } else {
                          jsonFeedback["ps"] = -1;
                        }
                        serializeJson(jsonFeedback, outputString);
                        Serial.println(outputString);
                        Serial0.println(outputString);
                        break;
  case CMD_CHANGE_ID:
                        jointsCtrl.changeID(jsonCmdInput["old_id"], 
                                            jsonCmdInput["new_id"]);
                        break;
  case CMD_SET_MIDDLE:
                        if (jointsCtrl.setMiddle(jsonCmdInput["id"])) {
                          Serial0.println("Joint middle set successfully.");
                          Serial.println("Joint middle set successfully.");
                        } else {
                          Serial0.println("Failed to set joint middle.");
                          Serial.println("Failed to set joint middle.");
                        }
                        break;
  case CMD_MOVE_MIDDLE:
                        jointsCtrl.moveMiddle(jsonCmdInput["id"]);
                        break;
  case CMD_TORQUE_LOCK:
                        jointsCtrl.torqueLock(jsonCmdInput["id"], 
                                              jsonCmdInput["state"]);
                        break;
  case CMD_STEPS_CTRL_SC:
                        jointsCtrl.stepsCtrlSC(jsonCmdInput["id"], 
                                               jsonCmdInput["pos"], 
                                               jsonCmdInput["time"], 
                                               jsonCmdInput["spd"], 
                                               jsonCmdInput["move"]);
                        break;
  case CMD_STEPS_CTRL_SMST:
                        Serial0.println("...");
                        Serial.println("...");
                        break;
  case CMD_STEPS_CTRL_HL:
                        Serial0.println("...");
                        Serial.println("...");
                        break;
  case CMD_ANGLE_CTRL_SC:
                        jointsCtrl.angleCtrlSC(jsonCmdInput["id"], 
                                               jsonCmdInput["mid"], 
                                               jsonCmdInput["ang"], 
                                               jsonCmdInput["spd"], 
                                               jsonCmdInput["move"]);
                        break;

  // case CMD_HUB_MOTOR_CTRL:
  //                       jointsCtrl.hubMotorCtrl(jsonCmdInput["spd_1"], 
  //                                               jsonCmdInput["spd_2"], 
  //                                               jsonCmdInput["spd_3"], 
  //                                               jsonCmdInput["spd_4"]);
  //                       break;
  




	// case CMD_JOINT_MIDDLE:
  //                       // bodyCtrl.jointMiddle();
  //                       break;
  // case CMD_RELEASE_TORQUE:
  //                       // bodyCtrl.releaseTorque();
  //                       break;
  // case CMD_SINGLE_SERVO_CTRL:
  //                       // bodyCtrl.singleServoCtrl(jsonCmdInput["id"], 
  //                       //                          jsonCmdInput["goal"], 
  //                       //                          jsonCmdInput["time"], 
  //                       //                          jsonCmdInput["spd"]);
  //                       break;
  // case CMD_GET_JOINTS_ZERO:
  //                       // memcpy(jointsZeroPos, bodyCtrl.getJointsZeroPosArray(), sizeof(jointsZeroPos));
  //                       // for (int i = 0; i < 12; i++) {  
  //                       //   Serial.print("Joint ");
  //                       //   Serial.print(i);
  //                       //   Serial.print(": ");
  //                       //   Serial.println(jointsZeroPos[i]);
  //                       // }
  //                       break;
  // case CMD_SET_JOINTS_ZERO:
  //                       // for (int i = 0; i < 12; i++) {
  //                       //   jointsZeroPos[i] = jsonCmdInput["set"][i];
  //                       // }
  //                       // bodyCtrl.setJointsZeroPosArray(jointsZeroPos);
  //                       break;
  // case CMD_GET_CURRENT_POS:
  //                       // memcpy(jointsCurrentPos, bodyCtrl.getServoFeedback(), sizeof(jointsCurrentPos));
  //                       // jsonFeedback.clear();
  //                       // jsonFeedback["T"] = - CMD_GET_CURRENT_POS;
  //                       // for (int i = 0; i < 12; i++) {
  //                       //   jsonFeedback["fb"][i] = jointsCurrentPos[i];
  //                       // }
  //                       // serializeJson(jsonFeedback, outputString);
  //                       // Serial.println(outputString);
  //                       break;
  // case CMD_SET_CURRENT_POS_ZERO:
  //                       // bodyCtrl.setCurrentPosZero();
  //                       // memcpy(jointsZeroPos, bodyCtrl.getJointsZeroPosArray(), sizeof(jointsZeroPos));
  //                       // jsonFeedback.clear();
  //                       // jsonFeedback["T"] = CMD_SET_JOINTS_ZERO;
  //                       // for (int i = 0; i < 12; i++) {  
  //                       //   Serial.print("Joint ");
  //                       //   Serial.print(i);
  //                       //   Serial.print(": ");
  //                       //   Serial.println(jointsZeroPos[i]);
  //                       //   jsonFeedback["set"][i] = jointsZeroPos[i];
  //                       // }
  //                       // serializeJson(jsonFeedback, outputString);
  //                       // Serial.println(outputString);
  //                       // filesCtrl.appendStep("boot", outputString);
  //                       break;
  // case CMD_CTRL_JOINT_ANGLE:
  //                       // bodyCtrl.jointAngle(jsonCmdInput["joint"], jsonCmdInput["angle"]);
  //                       // bodyCtrl.moveTrigger();
  //                       break;
  // case CMD_CTRL_JOINT_RAD:
  //                       // bodyCtrl.jointRad(jsonCmdInput["joint"], jsonCmdInput["rad"]);
  //                       // bodyCtrl.moveTrigger();
  //                       break;


  // led/screen/button/buzzer ctrl
	// case CMD_SET_COLOR: 
  //                       led.setColor(jsonCmdInput["set"][0], 
  //                                    jsonCmdInput["set"][1], 
  //                                    jsonCmdInput["set"][2], 
  //                                    jsonCmdInput["set"][3]);
	// 											break;
  case CMD_DISPLAY_SINGLE:
                        screenCtrl.changeSingleLine(jsonCmdInput["line"], 
                                                    jsonCmdInput["text"], 
                                                    jsonCmdInput["update"]);
                        break;
  case CMD_DISPLAY_UPDATE:
                        screenCtrl.updateFrame();
                        break;
                        
  case CMD_DISPLAY_FRAME:
                        screenCtrl.changeSingleLine(1, jsonCmdInput["l1"], false);
                        screenCtrl.changeSingleLine(2, jsonCmdInput["l2"], false);
                        screenCtrl.changeSingleLine(3, jsonCmdInput["l3"], false);
                        screenCtrl.changeSingleLine(4, jsonCmdInput["l4"], true);
                        break;
  case CMD_DISPLAY_CLEAR:
                        screenCtrl.clearDisplay();
                        break;
  case CMD_BUZZER_CTRL:
                        tone(BUZZER_PIN, jsonCmdInput["freq"]);
                        breakloop = false;
                        tuneStartTime = millis();
                        while (millis() - tuneStartTime < jsonCmdInput["duration"]) {
                          if (breakloop) {
                            noTone(BUZZER_PIN);
                            digitalWrite(BUZZER_PIN, HIGH);
                            break;
                          }
                        }
                        noTone(BUZZER_PIN);
                        digitalWrite(BUZZER_PIN, HIGH);
                        break;
  // case CMD_BUTTONS:
  //                       buttonInteractionCtrl(jsonCmdInput["L"],
  //                                             jsonCmdInput["I0"],
  //                                             jsonCmdInput["I1"],
  //                                             jsonCmdInput["I2"]);
  //                       break;



  case CMD_SCAN_FILES:
                        filesCtrl.scan();
                        break;
  case CMD_CREATE_MISSION:
                        filesCtrl.createMission(jsonCmdInput["name"], 
                                                jsonCmdInput["intro"]);
                        break;
  case CMD_FORMAT_FLASH:
                        filesCtrl.flash();
                        break;
  case CMD_MISSION_CONTENT:
                        filesCtrl.missionContent(jsonCmdInput["name"]);
                        break;
  case CMD_APPEND_SETP_JSON:
                        filesCtrl.appendStep(jsonCmdInput["name"],
                                             jsonCmdInput["json"]);
                        break;
  case CMD_INSERT_SETP_JSON:
                        filesCtrl.insertStep(jsonCmdInput["name"],
                                             jsonCmdInput["step"],
                                             jsonCmdInput["json"]);
                        break;
  case CMD_REPLACE_SETP_JSON:
                        filesCtrl.replaceStep(jsonCmdInput["name"],
                                              jsonCmdInput["step"],
                                              jsonCmdInput["json"]);
                        break;
  case CMD_DELETE_SETP:
                        filesCtrl.deleteStep(jsonCmdInput["name"],
                                             jsonCmdInput["step"]);
                        break;
  case CMD_RUN_STEP:    
                        runStep(jsonCmdInput["name"], jsonCmdInput["step"]);
                        break;
  case CMD_RUN_MISSION: 
                        runMission(jsonCmdInput["name"], 
                                   jsonCmdInput["interval"], 
                                   jsonCmdInput["loop"]);
                        break;
  case CMD_DELETE_MISSION:
                        filesCtrl.deleteMission(jsonCmdInput["name"]);
                        break;
  


  case CMD_SET_WIFI_MODE:
                        if (jsonCmdInput["mode"] == 0) {
                          wireless.setWifiMode(jsonCmdInput["mode"], "", "", 0, "", "");
                          jsonFeedback.clear();
                          jsonFeedback["T"] = CMD_SET_WIFI_MODE;
                          jsonFeedback["mode"] = 0;
                          serializeJson(jsonFeedback, outputString);
                          filesCtrl.checkReplaceStep("boot", outputString);
                        } else if (jsonCmdInput["mode"] == 1) {
                          if (wireless.setWifiMode(jsonCmdInput["mode"], 
                                                   jsonCmdInput["ap_ssid"], 
                                                   jsonCmdInput["ap_password"], 
                                                   jsonCmdInput["channel"], 
                                                   jsonCmdInput["sta_ssid"], 
                                                   jsonCmdInput["sta_password"])) {
                              jsonFeedback.clear();
                              jsonFeedback["T"] = CMD_SET_WIFI_MODE;
                              jsonFeedback["mode"] = 1;
                              jsonFeedback["ap_ssid"] = jsonCmdInput["ap_ssid"];
                              jsonFeedback["ap_password"] = jsonCmdInput["ap_password"];
                              jsonFeedback["channel"] = jsonCmdInput["channel"];
                              jsonFeedback["sta_ssid"] = jsonCmdInput["sta_ssid"];
                              jsonFeedback["sta_password"] = jsonCmdInput["sta_password"];
                              serializeJson(jsonFeedback, outputString);
                              filesCtrl.checkReplaceStep("boot", outputString);
                          } else {
                              jsonFeedback.clear();
                              jsonFeedback["T"] = CMD_SET_WIFI_MODE;
                              jsonFeedback["mode"] = 1;
                              jsonFeedback["ap_ssid"] = jsonCmdInput["ap_ssid"];
                              jsonFeedback["ap_password"] = jsonCmdInput["ap_password"];
                              jsonFeedback["channel"] = jsonCmdInput["channel"];
                              jsonFeedback["sta_ssid"] = "";
                              jsonFeedback["sta_password"] = "";
                              serializeJson(jsonFeedback, outputString);
                              filesCtrl.checkReplaceStep("boot", outputString);
                          }
                        }
                        break;
  case CMD_WIFI_INFO:
                        outputString = filesCtrl.findCmdByType("boot", CMD_SET_WIFI_MODE);
                        Serial.println(outputString);
                        Serial0.println(outputString);
                        break;
  case CMD_GET_AP_IP:
                        outputString = wireless.getAPIP();
                        Serial.println(outputString);
                        Serial0.println(outputString);
                        break;
  case CMD_GET_STA_IP:
                        outputString = wireless.getSTAIP();
                        Serial.println(outputString);
                        Serial0.println(outputString);
                        break;



  case CMD_INIT_ESP_NOW:
                        wireless.espnowInit(jsonCmdInput["longrange"]);
                        break;
  case CMD_SET_ESP_NOW_MODE:
                        wireless.setEspNowMode(jsonCmdInput["mode"]);
                        break;
  case CMD_GET_MAC:
                        outputString = wireless.macToString(wireless.getMac());
                        Serial.println(outputString);
                        Serial0.println(outputString);
                        break;
  case CMD_ESP_NOW_SEND:
                        wireless.sendEspNow(jsonCmdInput["mac"], jsonCmdInput["data"]);
                        break;
  case CMD_ADD_MAC:
                        wireless.addMacToPeerString(jsonCmdInput["mac"]);
                        break;



  case CMD_ESP32_REBOOT:
                        ESP.restart();
                        break;
  case CMD_CLEAR_NVS:
                        nvs_flash_erase();
                        delay(1000);
                        nvs_flash_init();
                        break;
  }
}



// USB CDC receive callback
void tud_cdc_rx_cb(uint8_t itf) {
  static String receivedData;
  char buffer[256];
  uint32_t count = tud_cdc_read(buffer, sizeof(buffer));

  for (uint32_t i = 0; i < count; i++) {
    char receivedChar = buffer[i];
    receivedData += receivedChar;

    // Detect the end of the JSON string based on a specific termination character
    if (receivedChar == '\n') {
      // Now we have received the complete JSON string
      DeserializationError err = deserializeJson(jsonCmdReceive, receivedData);
      if (err == DeserializationError::Ok) {
        if (InfoPrint == 1) {
          USBSerial.print(receivedData);
        }
        newCmdReceived = true;
      } else {
        // Handle JSON parsing error here
        if (InfoPrint == 1) {
          USBSerial.print("JSON parsing error: ");
          USBSerial.println(err.c_str());
        }
      }
      // Reset the receivedData for the next JSON string
      receivedData = "";
    }
  }
}



void serialCtrl() {
  static String receivedData;

  while (Serial0.available() > 0) {
    char receivedChar = Serial0.read();
    receivedData += receivedChar;

    // Detect the end of the JSON string based on a specific termination character
    if (receivedChar == '\n') {
      // Now we have received the complete JSON string
      DeserializationError err = deserializeJson(jsonCmdReceive, receivedData);
      if (err == DeserializationError::Ok) {
  			if (InfoPrint == 1) {
  				Serial0.print(receivedData);
  			}
        jsonCmdReceiveHandler(jsonCmdReceive);
      } else {
        // Handle JSON parsing error here
        if (InfoPrint == 1) {
          Serial0.print("JSON parsing error: ");
          Serial0.println(err.c_str());
        }
      }
      // Reset the receivedData for the next JSON string
      receivedData = "";
    }
  }
}

int spd_mode = 1; // 1,2,3
int spd_fb = 0;
int spd_lr = 0;

void loop() {
  // unsigned long startTime = micros(); // Record the start time in microseconds
  serialCtrl();


  // if (sbus.Read()) {
  //   /* Grab the received data */
  //   sbusData = sbus.data();
  //   /* Display the received data */
  //   for (int8_t i = 0; i < sbusData.NUM_CH; i++) {
  //     Serial.print(sbusData.ch[i]);
  //     Serial.print("\t");
  //   }
  //   /* Display lost frames and failsafe data */
  //   Serial.print(sbusData.lost_frame);
  //   Serial.print("\t");
  //   Serial.println(sbusData.failsafe);

  //   if (sbusData.ch[4] < 300) {
  //     spd_mode = 1;
  //   } else if (sbusData.ch[4] == 1002) {
  //     spd_mode = 2;
  //   } else if (sbusData.ch[4] > 1700) {
  //     spd_mode = 3;
  //   }

  //   spd_fb = round(jointsCtrl.mapDouble(sbusData.ch[2], 282, 1722, -2000 * spd_mode, 2000 * spd_mode));
  //   spd_lr = round(jointsCtrl.mapDouble(sbusData.ch[3], 282, 1722, -2000, 2000));
    
  //   jointsCtrl.hubMotorCtrl(spd_fb + spd_lr, -(-spd_fb + spd_lr), -spd_fb + spd_lr, spd_fb + spd_lr);
  // }

  

  // unsigned long endTime = micros(); // Record the end time in microseconds
  // USBSerial.print("serialCtrl execution time: ");
  // USBSerial.print(endTime - startTime);
  // USBSerial.println(" µs");
  
  // uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  // const char* message = "{\"T\":401}";
  // wireless.sendEspNow(broadcastAddress, message);
  // delay(1000);

  // if (buttonPressFlag && !buttonState) {
  //   switch (buttonPress) {
  //     case BUTTON_UP:
  //       Serial0.println("UP");
  //       buttonState = true;
  //       wireless.sendEspNow("FF:FF:FF:FF:FF:FF", 
  //         "{\"T\":202,\"line\":1,\"text\":\"UP\",\"update\":4}");
  //       break;
  //     case BUTTON_DOWN:
  //       Serial0.println("DOWN");
  //       buttonState = true;
  //       wireless.sendEspNow("FF:FF:FF:FF:FF:FF", 
  //         "{\"T\":202,\"line\":1,\"text\":\"DOWN\",\"update\":4}");
  //       break;
  //     case BUTTON_LEFT:
  //       Serial0.println("LEFT");
  //       buttonState = true;
  //       wireless.sendEspNow("FF:FF:FF:FF:FF:FF", 
  //         "{\"T\":202,\"line\":1,\"text\":\"LEFT\",\"update\":4}");
  //       break;
  //     case BUTTON_RIGHT:
  //       Serial0.println("RIGHT");
  //       buttonState = true;
  //       wireless.sendEspNow("FF:FF:FF:FF:FF:FF", 
  //         "{\"T\":202,\"line\":1,\"text\":\"RIGHT\",\"update\":4}");
  //       break;
  //     case BUTTON_OK:
  //       Serial0.println("OK");
  //       buttonState = true;
  //       wireless.sendEspNow("FF:FF:FF:FF:FF:FF", 
  //         "{\"T\":202,\"line\":1,\"text\":\"OK\",\"update\":4}");
  //       break;
  //   }
  //   buttonPressFlag = false;
  // }

  // interaction();

  // jsonFeedback.clear();
  // jsonFeedback["T"] = CMD_DATA_RECV_TEST;
  // if (wireless.sendEspNowJson(broadcastAddress, jsonFeedback)) {
  //   data_send_times++;
  //   data_screen_test_update();
  // }
  // delay(3000);


  if (newCmdReceived) {
    jsonCmdReceiveHandler(jsonCmdReceive);
    newCmdReceived = false;
    jsonCmdReceive.clear();
  }
}

