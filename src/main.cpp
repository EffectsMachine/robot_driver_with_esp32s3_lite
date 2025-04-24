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

int jointsZeroPos[JOINTS_NUM];
int jointsCurrentPos[JOINTS_NUM];
double jointsGoalBuffer[JOINTS_NUM];
double xbzgIKFeedback[JOINTS_NUM + 1];

void jsonCmdReceiveHandler(const JsonDocument& jsonCmdInput);
void runMission(String missionName, int intervalTime, int loopTimes);

unsigned long startTime;
unsigned long endTime;

#ifdef UART0_AS_SBUS
bfs::SbusRx sbus(&Serial0, 44, 43, true);
bfs::SbusData sbusData;
int spd_mode = 1; // 1,2,3
int spd_fb = 0;
int spd_lr = 0;
#endif

void msg(String msgStr, bool newLine = true) {
  if (uartMsgFlag) {
    if (newLine) {
      Serial0.println(msgStr);
    } else {
      Serial0.print(msgStr);
    }
  }
  if (usbMsgFlag) {
    if (newLine) {
      Serial.println(msgStr);
    } else {
      Serial.print(msgStr);
    }
  }
}

void getMsgStatus() {
  jsonFeedback.clear();
  jsonFeedback["T"] = CMD_SET_MSG_OUTPUT;
  jsonFeedback["echo"] = echoMsgFlag;
  jsonFeedback["uart"] = uartMsgFlag;
  jsonFeedback["usb"]  = usbMsgFlag;
  serializeJson(jsonFeedback, outputString);
  msg(outputString);
}

void buttonBuzzer() {
  tone(BUZZER_PIN, 2000);
  delay(5);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, HIGH);
}

// --- --- --- SETUP --- --- ---
void setup() {
#ifdef UART0_AS_SBUS
    sbus.Begin();
    msg("S.BUS mode initialized.");
#else
    Serial0.begin(ESP32S3_BAUD_RATE);
    msg("Serial0 initialized for normal communication.");
    while (!Serial0) {}
#endif

  Wire.begin(IIC_SDA, IIC_SCL);
  Wire.setClock(400000);
  msg("device starting...");

  // buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  buttonBuzzer();

  // fake args, it will be ignored by the USB stack, default baudrate is 12Mbps
  USBSerial.begin(ESP32S3_BAUD_RATE);
  USB.begin();
  msg("ESP32-S3 USB CDC DONE!");

  // led.init();
  filesCtrl.init();

  jointsCtrl.init(BUS_SERVO_BAUD_RATE);
  jointsCtrl.setJointType(JOINT_TYPE_SC);
  jointsCtrl.setEncoderStepRange(1024, 220);
  delay(1000);
  jointsCtrl.moveMiddle(254);

  // screenCtrl.init();
  // screenCtrl.displayText("LYgion", 0, 0, 2);
  // screenCtrl.displayText("Robotics", 0, 16, 2);

  if(!filesCtrl.checkMission("boot")) {
    filesCtrl.createMission("boot", "this is the boot mission.");
    // filesCtrl.appendStep("boot", "{\"T\":400,\"mode\":1,\"ap_ssid\":\"LYgion\",\"ap_password\":\"12345678\",\"channel\":1,\"sta_ssid\":\"\",\"sta_password\":\"\"}");
  } 
  // runMission("boot", 0, 1);

  // esp-now init
  // wireless.espnowInit(false);
  // wireless.espnowInit(true);
  // wireless.setJsonCommandCallback(jsonCmdReceiveHandler);

  // wireless.addMacToPeer(broadcastAddress);  
  // jsonFeedback.clear();
  // jsonFeedback["T"] = CMD_DISPLAY_SINGLE;
  // jsonFeedback["line"] = 1;
  // jsonFeedback["text"] = "Lygion Robotics";
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

  // buttonUp.begin();
  // buttonUp.onPress([]() {
  //   Serial0.println("Button Up Pressed");
  //   screenCtrl.changeSingleLine(1, "Button Up Pressed", true);
  // });
  // buttonUp.onRelease([]() {
  //   Serial0.println("Button Up Released");
  //   screenCtrl.changeSingleLine(1, "Button Up Released", true);
  // });
  // buttonUp.enable();
}



bool runStep(String missionName, int step) {
  outputString = filesCtrl.readStep(missionName, step);
  err = deserializeJson(jsonCmdReceive, outputString);
  if (err == DeserializationError::Ok) {
    jsonCmdReceiveHandler(jsonCmdReceive);
    return true;
  } else {
    msg("JSON parsing error (this is a normal output when booting or running Mission): ");
    msg(err.c_str());
    return false;
  }
}



void runMission(String missionName, int intervalTime, int loopTimes) {
  intervalTime = intervalTime - timeOffset;
  if (intervalTime < 0) {intervalTime = 0;}
  int j = 1;
  while (true) {
    msg("Running loop: ", false);
    msg(String(j));
    int i = 1;
    while (true) {
      if (Serial0.available() > 0) {
        msg("Mission interrupted.");
        break;
      }
      if (breakloop) {
        breakloop = false;
        break;
      }
      if (runStep(missionName, i)) {
        msg("Step: ", false);
        msg(String(i));
        i++;
        delay(intervalTime);
      } else {
        msg("Mission Completed.");
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
  breakloop = false;
  cmdType = jsonCmdInput["T"].as<int>();
  switch(cmdType){
  // joints ctrl
  case CMD_SET_JOINTS_BAUD:
                        jointsCtrl.setBaudRate(jsonCmdInput["baud"]);
                        break;
  case CMD_SET_JOINTS_TYPE:
                        if (jointsCtrl.setJointType(jsonCmdInput["type"])) {
                          msg("Joint type set successfully.");
                        } else {
                          msg("Failed to set joint type.");
                        }
                        break;
  case CMD_SET_ENCODER:
                        if (jointsCtrl.setEncoderStepRange(jsonCmdInput["steps"], jsonCmdInput["angle"])) {
                          msg("Encoder step range set successfully.");
                        } else {
                          msg("Failed to set encoder step range.");
                        }
                        break;
  case CMD_SINGLE_FEEDBACK:
                        jointFeedback = jointsCtrl.singleFeedBack(jsonCmdInput["id"]);
                        jsonFeedback.clear();
                        jsonFeedback["T"] = -CMD_SINGLE_FEEDBACK;
                        if (jointFeedback[0] == -1) {
                          jsonFeedback["ps"] = -1;
                          serializeJson(jsonFeedback, outputString);
                          msg(outputString);
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
                          msg(outputString);
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
                        msg(outputString);
                        break;
  case CMD_CHANGE_ID:
                        jointsCtrl.changeID(jsonCmdInput["old_id"], 
                                            jsonCmdInput["new_id"]);
                        break;
  case CMD_SET_MIDDLE:
                        if (jointsCtrl.setMiddle(jsonCmdInput["id"])) {
                          msg("Joint middle set successfully.");
                        } else {
                          msg("Failed to set joint middle.");
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
                        // void
                        break;
  case CMD_STEPS_CTRL_HL:
                        // void
                        break;
  case CMD_ANGLE_CTRL_SC:
                        jointsCtrl.angleCtrlSC(jsonCmdInput["id"], 
                                               jsonCmdInput["mid"], 
                                               jsonCmdInput["ang"], 
                                               jsonCmdInput["spd"], 
                                               jsonCmdInput["move"]);
                        break;
  case CMD_RAD_CTRL_SC:
                        jointsCtrl.radCtrlSC(jsonCmdInput["id"],
                                             jsonCmdInput["mid"],
                                             jsonCmdInput["rad"],
                                             jsonCmdInput["spd"],
                                             jsonCmdInput["move"]);
                        break;
  case CMD_MOVE_TRIGGER:
                        jointsCtrl.moveTrigger();
                        break;



  // --- --- --- for applications: LyLinkArm --- --- ---
  case CMD_GET_JOINTS_ZERO:
                        memcpy(jointsZeroPos, jointsCtrl.getJointsZeroPosArray(), sizeof(jointsZeroPos));
                        jsonFeedback.clear();
                        jsonFeedback["T"] = -CMD_GET_JOINTS_ZERO;
                        for (int i = 0; i < JOINTS_NUM; i++) {  
                            jsonFeedback["pos"][i] = jointsZeroPos[i];
                        }
                        serializeJson(jsonFeedback, outputString);
                        msg(outputString);
                        break;
  case CMD_SET_JOINTS_ZERO:
                        for (int i = 0; i < JOINTS_NUM; i++) {
                            jointsZeroPos[i] = jsonCmdInput["pos"][i];
                            
                        }
                        jointsCtrl.setJointsZeroPosArray(jointsZeroPos);
                        break;
  case CMD_GET_LINK_ARM_POS:
                        memcpy(jointsCurrentPos, jointsCtrl.getLinkArmPosSC(), sizeof(jointsCurrentPos));
                        jsonFeedback.clear();
                        jsonFeedback["T"] = -CMD_GET_LINK_ARM_POS;
                        for (int i = 0; i < JOINTS_NUM; i++) {
                            jsonFeedback["pos"][i] = jointsCurrentPos[i];
                        }
                        serializeJson(jsonFeedback, outputString);
                        msg(outputString);
                        break;
  case CMD_SET_LINK_ARM_ZERO:
                        jointsCtrl.setCurrentSCPosMiddle();
                        memcpy(jointsCurrentPos, jointsCtrl.getLinkArmPosSC(), sizeof(jointsCurrentPos));
                        jsonFeedback.clear();
                        jsonFeedback["T"] = CMD_SET_JOINTS_ZERO;
                        for (int i = 0; i < JOINTS_NUM; i++) {
                            jsonFeedback["pos"][i] = jointsCurrentPos[i];
                        }
                        serializeJson(jsonFeedback, outputString);
                        msg(outputString);
                        break;
  case CMD_LINK_ARM_SC_JOINTS_CTRL_ANGLE:
                        for (int i = 0; i < JOINTS_NUM; i++) {
                            jointsGoalBuffer[i] = jsonCmdInput["ang"][i];
                        }
                        jointsCtrl.linkArmSCJointsCtrlAngle(jointsGoalBuffer);
                        break;
  case CMD_LINK_ARM_SC_JOINTS_CTRL_RAD:
                        for (int i = 0; i < JOINTS_NUM; i++) {
                            jointsGoalBuffer[i] = jsonCmdInput["rad"][i];
                        }
                        jointsCtrl.linkArmSCJointsCtrlRad(jointsGoalBuffer);
                        break;
  case CMD_XYZG_CTRL:
                        if(!jointsCtrl.linkArmSpaceIK(jsonCmdInput["xyzg"][0],
                                                      jsonCmdInput["xyzg"][1],
                                                      jsonCmdInput["xyzg"][2],
                                                      jsonCmdInput["xyzg"][3])) {
                        jsonFeedback.clear();
                        jsonFeedback["T"] = -CMD_XYZG_CTRL;
                        jsonFeedback["ik"] = -1;
                        jsonFeedback["xyzg"] = jsonCmdInput["xyzg"];
                        serializeJson(jsonFeedback, outputString);
                        msg(outputString);
                        }
                        break;
  case CMD_UI_ABS_CTRL:
                        memcpy(xbzgIKFeedback, 
                               jointsCtrl.linkArmUIIK(jsonCmdInput["rbzg"][0],
                                                      jsonCmdInput["rbzg"][1],
                                                      jsonCmdInput["rbzg"][2],
                                                      jsonCmdInput["rbzg"][3]), 
                               sizeof(jointsCurrentPos));
                        if (xbzgIKFeedback[0] == -1) {
                          jsonFeedback.clear();
                          jsonFeedback["T"] = -CMD_UI_ABS_CTRL;
                          jsonFeedback["ik"] = xbzgIKFeedback[0];
                          for (int i = 0; i < JOINTS_NUM; i++) {
                            jsonFeedback["xbzg"][i] = xbzgIKFeedback[i + 1];
                          }
                          serializeJson(jsonFeedback, outputString);
                          msg(outputString);
                        }
                        break;






  case CMD_HUB_MOTOR_CTRL:
                        jointsCtrl.hubMotorCtrl(jsonCmdInput["A"], 
                                                jsonCmdInput["B"], 
                                                jsonCmdInput["C"], 
                                                jsonCmdInput["D"]);
                        break;
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
                        tuneStartTime = millis();
                        while (millis() - tuneStartTime < jsonCmdInput["duration"]) {
                          if (breakloop) {
                            breakloop = false;
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
                        msg(outputString);
                        break;
  case CMD_GET_AP_IP:
                        outputString = wireless.getAPIP();
                        msg(outputString);
                        break;
  case CMD_GET_STA_IP:
                        outputString = wireless.getSTAIP();
                        msg(outputString);
                        break;



  case CMD_INIT_ESP_NOW:
                        wireless.espnowInit(jsonCmdInput["longrange"]);
                        break;
  case CMD_SET_ESP_NOW_MODE:
                        wireless.setEspNowMode(jsonCmdInput["mode"]);
                        break;
  case CMD_GET_MAC:
                        outputString = wireless.macToString(wireless.getMac());
                        msg(outputString);
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
  case CMD_GET_MSG_OUTPUT:
                        getMsgStatus();
                        break;
  case CMD_SET_MSG_OUTPUT:
                        echoMsgFlag = jsonCmdInput["echo"];
                        uartMsgFlag = jsonCmdInput["uart"];
                        usbMsgFlag  = jsonCmdInput["usb"];
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
        if (echoMsgFlag) {
          msg(receivedData, false);
        }
        newCmdReceived = true;
      } else {
        // Handle JSON parsing error here
        msg("JSON parsing error: ", false);
        msg(err.c_str());
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
        if (echoMsgFlag) {
          msg(receivedData, false);
        }
        jsonCmdReceiveHandler(jsonCmdReceive);
      } else {
        // Handle JSON parsing error here
        msg("JSON parsing error: ", false);
        msg(err.c_str());
      }
      // Reset the receivedData for the next JSON string
      receivedData = "";
    }
  }
}

void loop() {
  serialCtrl();

  // unsigned long startTime = micros(); // Record the start time in microseconds
  // // jointsCtrl.linkArmPlaneIK(LINK_AB, sqrt(pow(LINK_BF_1, 2) + pow(LINK_BF_2, 2)));
  // unsigned long endTime = micros(); // Record the end time in microseconds
  // Serial.print("Execution time: ");
  // Serial.print(endTime - startTime);
  // Serial.println(" µs");

#ifdef UART0_AS_SBUS
  if (sbus.Read()) {
    /* Grab the received data */
    sbusData = sbus.data();
    /* Display the received data */
    for (int8_t i = 0; i < sbusData.NUM_CH; i++) {
      Serial.print(sbusData.ch[i]);
      Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    Serial.print(sbusData.lost_frame);
    Serial.print("\t");
    Serial.println(sbusData.failsafe);

    if (sbusData.ch[4] < 300) {
      spd_mode = 1;
    } else if (sbusData.ch[4] == 1002) {
      spd_mode = 2;
    } else if (sbusData.ch[4] > 1700) {
      spd_mode = 3;
    }

    spd_fb = round(jointsCtrl.mapDouble(sbusData.ch[2], 282, 1722, -2000 * spd_mode, 2000 * spd_mode));
    spd_lr = round(jointsCtrl.mapDouble(sbusData.ch[3], 282, 1722, -2000, 2000));
    
    jointsCtrl.hubMotorCtrl(spd_fb + spd_lr, -(-spd_fb + spd_lr), -spd_fb + spd_lr, spd_fb + spd_lr);
  }
#endif
  

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

