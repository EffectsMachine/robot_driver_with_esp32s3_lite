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

#include "Config.h"
#include "jointsCtrl.h"
#include "FilesCtrl.h"
#include "ScreenCtrl.h"
#include "Wireless.h"

JsonDocument jsonCmdReceive;
JsonDocument jsonFeedback;
/*
Declare USBSerial as an instance of USBCDC
Serial is used for USB CDC communication
Serial0 is used for UART communication
*/
USBCDC USBSerial;
DeserializationError err;
String outputString;
JointsCtrl jointsCtrl;
FilesCtrl filesCtrl;
ScreenCtrl screenCtrl;
Wireless wireless;

#include "buttonUI.h"

bool newCmdReceived = false;
bool breakloop = false;
unsigned long tuneStartTime;
int* jointFeedback;

int jointsZeroPos[JOINTS_NUM] = {519, 471, 484, 514};
int jointsCurrentPos[JOINTS_NUM];
double jointFBRads[JOINTS_NUM];
int jointFBTorques[JOINTS_NUM];
double jointsGoalBuffer[JOINTS_NUM];
double xyzgIKFeedback[JOINTS_NUM + 1];
double rbzgIKFeedback[JOINTS_NUM + 1];
double xyzgIK[JOINTS_NUM + 1];
double rbzgIK[JOINTS_NUM + 1];

void jsonCmdReceiveHandler(const JsonDocument& jsonCmdInput);
void runMission(String missionName, int intervalTime, int loopTimes);

unsigned long startTime;
unsigned long endTime;

#ifdef UART0_AS_SBUS
bfs::SbusRx sbus(&Serial0, 44, 43, true);
bfs::SbusData sbusData;
#endif

void msg(String msgStr, bool newLine = true) {
#ifndef UART0_AS_SBUS
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
#endif
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


/**
 * @brief Setup function to initialize the device and its peripherals.
 * 
 * - Waits for the internal capacitors of the joints to charge.
 * - Initializes communication interfaces (S.BUS or Serial0).
 * - Configures I2C communication with specified pins and clock speed.
 * - Sets up the buzzer and triggers a button buzzer action.
 * - Initializes USB CDC communication for the ESP32-S3.
 * - Configures and initializes joint control with specific parameters.
 * - Optionally initializes the screen controller for display (commented out).
 * - If file system usage is enabled, initializes the file system and checks/creates a boot mission.
 * - If ESP-NOW is enabled, initializes wireless communication and sets up a JSON command callback.
 */
void setup() {
  /*
  Waits for the internal capacitors of the joints to charge.
  */
  delay(1000);
  // delay(5000);

#ifdef UART0_AS_SBUS
    sbus.Begin();
    msg("S.BUS mode initialized.");
#else
    Serial0.begin(ESP32S3_BAUD_RATE);
    msg("Serial0 initialized for normal communication.");
    while (!Serial0) {}
#endif
  msg("Device starting...");

  Wire.begin(IIC_SDA, IIC_SCL);
  Wire.setClock(400000);

  // buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  buttonBuzzer();



  // fake args, it will be ignored by the USB stack, default baudrate is 12Mbps
  USBSerial.begin(ESP32S3_BAUD_RATE);
  USB.begin();
  msg("ESP32-S3 USB CDC DONE!");

  jointsCtrl.init(BUS_SERVO_BAUD_RATE);
  jointsCtrl.setJointType(JOINT_TYPE_SC);
  jointsCtrl.setEncoderStepRange(1024, 220);
  jointsCtrl.setJointsZeroPosArray(jointsZeroPos);
#ifdef USE_ROBOTIC_ARM
  double maxSpeedBuffer = jointsCtrl.getMaxJointsSpeed();
  jointsCtrl.setMaxJointsSpeed(0.1);
  jointsCtrl.linkArmFPVIK(LINK_AB + LINK_BF_1 + LINK_EF/2, 
                          0, 
                          LINK_BF_2,
                          0);
  msg("JointsCtrl initialized.");
  jointsCtrl.setMaxJointsSpeed(maxSpeedBuffer);
#endif

#ifdef USE_UI_CTRL
  screenCtrl.init();
  screenCtrl.displayText("LYgion", 0, 0, 2);
  screenCtrl.displayText("Robotics", 0, 16, 2);
  msg("ScreenCtrl initialized.");
#else
  msg("ScreenCtrl NOT initialized.");
#endif

#ifdef USE_FILE_SYSTEM
  filesCtrl.init();
  if(!filesCtrl.checkMission("boot")) {
    filesCtrl.createMission("boot", "this is the boot mission.");
    filesCtrl.appendStep("boot", "{\"T\":400,\"mode\":1,\"ap_ssid\":\"LYgion\",\"ap_password\":\"12345678\",\"channel\":1,\"sta_ssid\":\"\",\"sta_password\":\"\"}");
    msg("creat New mission: boot");
  } else {
    msg("boot mission already exists.");
  }
  runMission("boot", 0, 1);
  msg("File system initialized.");
#else
  msg("File system NOT initialized.");
#endif

#ifdef USE_ESP_NOW
  wireless.espnowInit(false);
  wireless.espnowInit(true);
  wireless.setJsonCommandCallback(jsonCmdReceiveHandler);
  msg("ESP-NOW initialized.");
#else
  msg("ESP-NOW NOT initialized.");
#endif

#ifndef USE_HUB_MOTORS
  msg("Hub motors NOT initialized.");
#endif

  // buttonUI

  
  /*
  Assign button events to menu actions
  must be after wifi function
  */
  menu_init_RD();
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
                        memcpy(xyzgIK,
                        jointsCtrl.linkArmSpaceIK(jsonCmdInput["xyzg"][0],
                                                  jsonCmdInput["xyzg"][1],
                                                  jsonCmdInput["xyzg"][2],
                                                  jsonCmdInput["xyzg"][3]),
                        sizeof(xyzgIK));
                        if (xyzgIK[0] == -1) {
                          jsonFeedback.clear();
                          jsonFeedback["T"] = -CMD_XYZG_CTRL;
                          jsonFeedback["ik"] = xyzgIK[0];
                          for (int i = 0; i < JOINTS_NUM; i++) {
                            jsonFeedback["xyzg"][i] = xyzgIK[i + 1];
                          }
                          serializeJson(jsonFeedback, outputString);
                          msg(outputString);
                        }
                        break;
  case CMD_FPV_ABS_CTRL:
                        memcpy(rbzgIKFeedback, 
                               jointsCtrl.linkArmFPVIK(jsonCmdInput["rbzg"][0],
                                                      jsonCmdInput["rbzg"][1],
                                                      jsonCmdInput["rbzg"][2],
                                                      jsonCmdInput["rbzg"][3]), 
                               sizeof(rbzgIKFeedback));
                        if (rbzgIKFeedback[0] == -1) {
                          jsonFeedback.clear();
                          jsonFeedback["T"] = -CMD_FPV_ABS_CTRL;
                          jsonFeedback["ik"] = rbzgIKFeedback[0];
                          for (int i = 0; i < JOINTS_NUM; i++) {
                            jsonFeedback["rbzg"][i] = rbzgIKFeedback[i + 1];
                          }
                          serializeJson(jsonFeedback, outputString);
                          msg(outputString);
                        }
                        break;
  case CMD_SMOOTH_XYZG_CTRL:
                        memcpy(xyzgIKFeedback, 
                          jointsCtrl.smoothXYZGCtrl(jsonCmdInput["xyzg"][0],
                                                    jsonCmdInput["xyzg"][1],
                                                    jsonCmdInput["xyzg"][2],
                                                    jsonCmdInput["xyzg"][3],
                                                    jsonCmdInput["spd"]), 
                          sizeof(xyzgIKFeedback));
                        if (xyzgIKFeedback[0] == -1) {
                          jsonFeedback.clear();
                          jsonFeedback["T"] = -CMD_SMOOTH_XYZG_CTRL;
                          jsonFeedback["ik"] = xyzgIKFeedback[0];
                          for (int i = 0; i < JOINTS_NUM; i++) {
                            jsonFeedback["xyzg"][i] = xyzgIKFeedback[i + 1];
                          }
                          serializeJson(jsonFeedback, outputString);
                          msg(outputString);
                        }
                        break;
  case CMD_SMOOTH_FPV_ABS_CTRL:
                        memcpy(rbzgIKFeedback, 
                               jointsCtrl.smoothFPVAbsCtrl(jsonCmdInput["rbzg"][0],
                                                           jsonCmdInput["rbzg"][1],
                                                           jsonCmdInput["rbzg"][2],
                                                           jsonCmdInput["rbzg"][3],
                                                           jsonCmdInput["spd"],
                                                           jsonCmdInput["br"]), 
                               sizeof(rbzgIKFeedback));
                        if (rbzgIKFeedback[0] == -1) {
                          jsonFeedback.clear();
                          jsonFeedback["T"] = -CMD_FPV_ABS_CTRL;
                          jsonFeedback["ik"] = rbzgIKFeedback[0];
                          for (int i = 0; i < JOINTS_NUM; i++) {
                            jsonFeedback["rbzg"][i] = rbzgIKFeedback[i + 1];
                          }
                          serializeJson(jsonFeedback, outputString);
                          msg(outputString);
                        }
                        break;
  case CMD_SET_MAX_JOINTS_SPEED:
                        jointsCtrl.setMaxJointsSpeed(jsonCmdInput["spd"]);
                        break;
  case CMD_GET_JOINT_FB_RADS:
                        memcpy(jointFBRads, jointsCtrl.getJointFBRads(), sizeof(jointFBRads));
                        jsonFeedback.clear();
                        jsonFeedback["T"] = -CMD_GET_JOINT_FB_RADS;
                        for (int i = 0; i < JOINTS_NUM; i++) {
                          jsonFeedback["fb-rads"][i] = jointFBRads[i];
                        }
                        serializeJson(jsonFeedback, outputString);
                        msg(outputString);
                        break;
  case CMD_GET_JOINT_GOAL_RADS:
                        memcpy(jointsGoalBuffer, jointsCtrl.getJointGoalRads(), sizeof(jointsGoalBuffer));
                        jsonFeedback.clear();
                        jsonFeedback["T"] = -CMD_GET_JOINT_GOAL_RADS;
                        for (int i = 0; i < JOINTS_NUM; i++) {
                          jsonFeedback["goal-rads"][i] = jointsGoalBuffer[i];
                        }
                        serializeJson(jsonFeedback, outputString);
                        msg(outputString);
                        break;
  case CMD_GET_XYZG_IK:
                        memcpy(xyzgIK, jointsCtrl.getXYZGIK(), sizeof(xyzgIK));
                        jsonFeedback.clear();
                        jsonFeedback["T"] = -CMD_GET_XYZG_IK;
                        for (int i = 0; i < JOINTS_NUM + 1; i++) {
                          jsonFeedback["xyzg"][i] = xyzgIK[i];
                        }
                        serializeJson(jsonFeedback, outputString);
                        msg(outputString);
                        break;
  case CMD_GET_RBZG_IK:
                        memcpy(rbzgIK, jointsCtrl.getRBZGIK(), sizeof(rbzgIK));
                        jsonFeedback.clear();
                        jsonFeedback["T"] = -CMD_GET_RBZG_IK;
                        for (int i = 0; i < JOINTS_NUM + 1; i++) {
                          jsonFeedback["rbzg"][i] = rbzgIK[i];
                        }
                        serializeJson(jsonFeedback, outputString);
                        msg(outputString);
                        break;
  case CMD_SET_LINK_ARM_FEEDBACK_FLAG:
                        if (jsonCmdInput["flag"] == 0) {
                          jointsCtrl.setLinkArmFeedbackFlag(false, jsonCmdInput["hz"]);
                        } else if (jsonCmdInput["flag"] == 1) {
                          jointsCtrl.setLinkArmFeedbackFlag(true, jsonCmdInput["hz"]);
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

    float speed_limit = 1.0;
    if (sbusData.ch[4] < 300) {
      speed_limit = 0.33;
    } else if (sbusData.ch[4] == 1002) {
      speed_limit = 0.66;
    } else if (sbusData.ch[4] > 1700) {
      speed_limit = 1.0;
    }

    float speed_input = constrain(float(sbusData.ch[2] - SBUS_MID)/SBUS_RAN, -1.0, 1.0);
    float turn_input = - constrain(float(sbusData.ch[3] - SBUS_MID)/SBUS_RAN, -1.0, 1.0);

    float left_speed = (speed_input) * speed_limit * 6000.0 - (turn_input * 0.33 * 6000.0);
    float right_speed = (speed_input) * speed_limit * 6000.0 + (turn_input * 0.33 * 6000.0);

    jointsCtrl.hubMotorCtrl(left_speed, -(right_speed), -right_speed, left_speed);
  }
#endif

#ifdef USE_ROBOTIC_ARM
  static unsigned long lastFeedbackTime = 0;
  if (jointsCtrl.linkArmFeedbackFlag && (millis() - lastFeedbackTime >= (1000 / jointsCtrl.linkArmFeedbackHz))) {
    lastFeedbackTime = millis();
    memcpy(jointFBRads, jointsCtrl.getJointFBRads(), sizeof(jointFBRads));
    memcpy(jointFBTorques, jointsCtrl.getLinkArmTorqueSC(), sizeof(jointFBTorques));
    jsonFeedback.clear();
    jsonFeedback["T"] = -CMD_GET_JOINT_FB_RADS;
    for (int i = 0; i < JOINTS_NUM; i++) {
      jsonFeedback["rads"][i] = String(jointFBRads[i], 3).toDouble();
      jsonFeedback["tors"][i] = jointFBTorques[i];
    }
    serializeJson(jsonFeedback, outputString);
    msg(outputString);
  }

  if (newCmdReceived) {
    jsonCmdReceiveHandler(jsonCmdReceive);
    newCmdReceived = false;
    jsonCmdReceive.clear();
  }
#endif
}

