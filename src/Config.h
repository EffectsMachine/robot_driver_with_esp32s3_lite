// --- PIN Configuration ---
// config for the RGB LED
#define RGB_PIN 26
#define RGB_NUM 2

// config for the bus servo
#define BUS_SERVO_RX 5
#define BUS_SERVO_TX 4

// pin for I2C
#define I2C_SDA 32
#define I2C_SCL 33

// pin for the buzzer
#define BUZZER_PIN 21

// pin for the debug
#define DEBUG_PIN 12

// IIC
#define IIC_SDA 6
#define IIC_SCL 7

// Buzzer
#define BUZZER_PIN 21



// --- Debug Configuration ---
// 1: print debug info
// 0: do not print debug info
static int InfoPrint = 1;

// serial baud rate
#define BAUD_RATE 921600

// processing time adjustment
static int timeOffset = 50;



// --- Command Configuration ---
// Joints Ctrl
// {"T":100,"baud":1000000}
// jointsCtrl.setBaudRate(1000000);
#define CMD_SET_JOINTS_BAUD 100

// {"T":101,"type":1}
// jointsCtrl.setJointType(1);
#define CMD_SET_JOINTS_TYPE 101

// {"T":102,"steps":4096,"angle":360}
// jointsCtrl.setEncoderStepRange(4096, 360);
#define CMD_SET_ENCODER 102

// {"T":103,"id":1}
// jointsCtrl.singleFeedBack(1);
#define CMD_SINGLE_FEEDBACK 103

// {"T":104,"id":1}
// jointsCtrl.ping(1);
#define CMD_PING 104

// {"T":105,"old_id":1,"new_id":2}
// jointsCtrl.changeID(1, 2);
#define CMD_CHANGE_ID 105

// {"T":106,"id":1}
// jointsCtrl.setMiddle(1);
#define CMD_SET_MIDDLE 106

// {"T":107,"id":1}
// jointsCtrl.moveMiddle(1);
#define CMD_MOVE_MIDDLE 107

// {"T":108,"id":1,"state":1}
// jointsCtrl.torqueLock(1, 1);
#define CMD_TORQUE_LOCK 108

// {"T":109,"id":1,"pos":512,"time":0,"spd":0,"move":1}
// jointsCtrl.stepsCtrlSC(1, 511, 0, 0, true);
#define CMD_STEPS_CTRL_SC 109

#define CMD_STEPS_CTRL_SMST 110
#define CMD_STEPS_CTRL_HL 111

// {"T":112,"id":1,"mid":512,"ang":10,"spd":0,"move":1}
// jointsCtrl.angleCtrlSC(1, 511, 10, 0, true);
#define CMD_ANGLE_CTRL_SC 112

#define CMD_ANGLE_CTRL_SMST 113
#define CMD_ANGLE_CTRL_HL 114

// {"T":115,"id":1,"mid":512,"rad":0.785,"spd":0,"move":1}
// jointsCtrl.radCtrlSC(1, 511, 0.785, 0, 1);
#define CMD_RAD_CTRL_SC 115

#define CMD_RAD_CTRL_SMST 116
#define CMD_RAD_CTRL_HL 117

// {"T":118}
// jointsCtrl.moveTrigger();
#define CMD_MOVE_TRIGGER 118


// --- --- --- for applications: LyLinkArm --- --- ---
// {"T":130}
#define CMD_GET_JOINTS_ZERO 130

// {"T":131,"pos":[512,512,512,512]}
// example LyLinkArm
// {"T":131,"pos":[497,505,286,557]} (LE-2)
// {"T":131,"pos":[519,471,484,514]} (LY-6V)
// jointsCtrl.setJointsZeroPosArray(int values[]);
#define CMD_SET_JOINTS_ZERO 131

// {"T":132}
// jointsCtrl.getLinkArmPosSC();
#define CMD_GET_LINK_ARM_POS 132

// {"T":133}
#define CMD_SET_LINK_ARM_ZERO 133

// {"T":134,"ang":[0,0,0,0]}
// gripper: -45-grip, +25-release 
// jointsCtrl.linkArmSCJointsCtrlAngle(double angles[]);
#define CMD_LINK_ARM_SC_JOINTS_CTRL_ANGLE 134

// {"T":135,"rad":[0,0,0,0]}
// jointsCtrl.linkArmSCJointsCtrlRad(double rads[]);
#define CMD_LINK_ARM_SC_JOINTS_CTRL_RAD 135

// {"T":136,"xyzg":[236.5,0,122.38,0]}
#define CMD_XYZG_CTRL 136

// {"T":137,"rbzg":[236.5,0,122.38,0]}
#define CMD_UI_ABS_CTRL 137



// steps * 0.06 = rpm
// {"T":180,"A":0,"B":0,"C":0,"D":0}
// jointsCtrl.hubMotorCtrl(0, 0, 0, 0);
#define CMD_HUB_MOTOR_CTRL 180

// // {"T":150}
// // #define 
// // {"T":101}
// #define CMD_JOINT_MIDDLE 151
// // Release Torque
// // {"T":102}
// #define CMD_RELEASE_TORQUE 102
// // single servo ctrl
// // {"T":103,"id":1,"goal":512,"time":0,"spd":0}
// #define CMD_SINGLE_SERVO_CTRL 103
// // get joints zero pos array - [Coupling function]
// // {"T":104}
// #define CMD_GET_JOINTS_ZERO 104
// // set joints zero pos array - [Coupling function]
// // {"T":105,"set":[512,512,512,512,512,512,512,512,512,512,512,512]}
// #define CMD_SET_JOINTS_ZERO 105
// // get the current pos of all servos - [Coupling function]
// // {"T":106}
// // feedback: {"T":-106,"fb":[512,512,512,512,512,512,512,512,512,512,512,512]}
// #define CMD_GET_CURRENT_POS 106
// // set the current pos as the zero pos
// // {"T":107}
// #define CMD_SET_CURRENT_POS_ZERO 107

// // ctrl single joint angle
// // {"T":108,"joint":1,"angle":45}
// #define CMD_CTRL_JOINT_ANGLE 108
// // ctrl single joint radian
// // {"T":109,"joint":1,"rad":0.785}
// #define CMD_CTRL_JOINT_RAD 109



// SMS/ST Servo Ctrl
// {"T":150}



// id 0 -> left LED
// id 1 -> right LED
// JSON cmds: [T:201, set:[id, r, g, b]]
// {"T":201,"set":[0,9,0,0]}
#define CMD_SET_COLOR 201
// ctrl oled display a single line
// updateFlag = 0 -> no update, updateFlag = 1 -> update
// {"T":202,"line":1,"text":"Hello, world!","update":1}
#define CMD_DISPLAY_SINGLE 202
// oled display update
// {"T":203}
#define CMD_DISPLAY_UPDATE 203
// ctrl oled display a frame
// {"T":204,"l1":"Hello, world!","l2":"Hello, world!","l3":"Hello, world!","l4":"Hello, world!"}
#define CMD_DISPLAY_FRAME 204
// oled display clear
// {"T":205}
#define CMD_DISPLAY_CLEAR 205
// buzzer ctrl
// {"T":206,"freq":1000,"duration":1000}
#define CMD_BUZZER_CTRL 206

// buttons
// {"T":207,"L":0,"I0":0,"I1":0,"I2":0}
#define CMD_BUTTONS 207



// scan files
// {"T":300}
#define CMD_SCAN_FILES 300
// create mission
// {"T":301,"name":"mission1","intro":"introduction for this mission file."}
#define CMD_CREATE_MISSION 301
// show the content of the mission
// {"T":302,"name":"boot"}
#define CMD_MISSION_CONTENT 302
// append a new json cmd to the mission file
// {"T":303,"name":"boot","json":"{\"T\":201,\"set\":[0,9,0,0]}"}
#define CMD_APPEND_SETP_JSON 303
// insert a new json cmd to the mission file under the stepNum
// {"T":304,"name":"boot","step":2,"json":"{\"T\":201,\"set\":[0,9,0,0]}"}
#define CMD_INSERT_SETP_JSON 304
// replace a new json cmd to the mission file under the stepNum
// {"T":305,"name":"boot","step":2,"json":"{\"T\":201,\"set\":[0,9,0,0]}"}
#define CMD_REPLACE_SETP_JSON 305
// delete a step from the mission file
// {"T":306,"name":"boot","step":2}
#define CMD_DELETE_SETP 306
// run a single step - [Coupling function]
// {"T":307,"name":"boot","step":2}
#define CMD_RUN_STEP 307
// run the whole mission - [Coupling function]
// {"T":308,"name":"boot","interval":1000,"loop":1}
#define CMD_RUN_MISSION 308
// delete mission
// {"T":309,"name":"boot"}
#define CMD_DELETE_MISSION 309
// format flash (clear all)
// {"T":399}
#define CMD_FORMAT_FLASH 399



// --- Wireless Configuration ---
// 0 -> none, 1[default] -> AP+STA
// {"T":400,"mode":1,"ap_ssid":"LYgion","ap_password":"12345678","channel":1,"sta_ssid":"ssid","sta_password":"password"}
#define CMD_SET_WIFI_MODE 400
// {"T":401}
#define CMD_WIFI_INFO 401
// {"T":402}
#define CMD_GET_AP_IP 402
// {"T":403}
#define CMD_GET_STA_IP 403

// {"T":410,"longrange":0}
#define CMD_INIT_ESP_NOW 410
// 0 -> no esp-now receive, 1 -> start esp-now[default]
// {"T":411,"mode":1}
#define CMD_SET_ESP_NOW_MODE 411
// {"T":412}
#define CMD_GET_MAC 412
// {"T":413,"mac":"FF,FF,FF,FF,FF,FF","data":"{\"T\":205,\"freq\":500,\"duration\":30}"}
#define CMD_ESP_NOW_SEND 413
// {"T":414,"mac":"FF:FF:FF:FF:FF:FF"}
#define CMD_ADD_MAC 414

// esp-now test:
// {"T":490,"mac":"FF:FF:FF:FF:FF:FF","data":"{\"T\":490}"}
// #define CMD_DATA_RECV_TEST 490


// --- System Configuration ---
// the time for ESP32 to reboot after receiving a reboot command
// {"T":600}
#define CMD_ESP32_REBOOT 600
// clear nvs
// {"T":601}
#define CMD_CLEAR_NVS 601
#define CMD_RESET_BOOT_MISSION 602