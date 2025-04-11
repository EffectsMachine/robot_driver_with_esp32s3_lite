#ifndef JOINTS_CTRL_H
#define JOINTS_CTRL_H

#include <SCServo.h>

#define JOINTS_NUM 4 // Number of joints
#define JOINTS_SC_ZERO_POS 512 // Default zero position for each joint
#define JOINTS_SC_MAX_POS 1023 // Maximum position for each joint

#define SERVO_ARRAY_0 31
#define SERVO_ARRAY_1 32
#define SERVO_ARRAY_2 33
#define SERVO_ARRAY_3 34

class JointsCtrl {
    private:
        SCSCL sc;

        // for applications: LyLinkArm
        int jointsZeroPos[JOINTS_NUM]; // array to store the zero position of each joint
        int jointsCurrentPos[JOINTS_NUM]; // array to store the current position of each joint
        int jointsGoalPos[JOINTS_NUM]; // array to store the goal position of each joint
        int jointID[JOINTS_NUM] = {SERVO_ARRAY_0, SERVO_ARRAY_1, SERVO_ARRAY_2, SERVO_ARRAY_3};

        // [0]ping status
        // [1]position
        // [2]speed
        // [3]load
        // [4]voltage
        // [5]temperature
        // [6]moving
        // [7]current
        int servoFeedback[8];

        u_int8_t jointType = 0; // 0: SC, 1: SMS/ST, 3: HL
    
    public:
        // for applications: LyLinkArm
        JointsCtrl() {
            // Initialize jointsZeroPos array
            for(int i = 0; i < JOINTS_NUM; i++) {
                jointsZeroPos[i] = JOINTS_SC_ZERO_POS; // or any other default value
                jointsCurrentPos[i] = JOINTS_SC_ZERO_POS; // or any other default value
                jointsGoalPos[i] = JOINTS_SC_ZERO_POS; // or any other default value
            }
        }

        void init(int baud);
        void setBaudRate(int baud);
        int* feedBack(u_int8_t id);
        void setJointType(u_int8_t type);
        void changeID(u_int8_t oldID, u_int8_t newID);
        bool setMiddle(u_int8_t id); // sc servo can not setMiddle

        void jointMiddle(u_int8_t id);
        void torqueLock(u_int8_t id, int state);
        void singleServoCtrl(u_int8_t id, int pos, int noidea, int speed);
        int* getJointsZeroPosArray();
        void setJointsZeroPosArray(int values[]);
        int* getServoFeedback();
        void setCurrentPosZero();
        double mapDouble(double x, double in_min, double in_max, double out_min, double out_max);
        void angleCtrl(u_int8_t id, double angleW);
        void radCtrl(u_int8_t id, double rad);
        void moveTrigger();
    
        // SMS/ST Servo Ctrl
        void wheelMode(int id);
        void servoMode(int id);
        void writePos(int id, int pos, int speed, int acc);
        void writeSpd(int id, int speed, int acc);
        void regWritePos(int id, int pos, int speed, int acc);
        void regMoveTrigger();
        void syncWritePos(int id[], int pos[], int speed[], int acc[], int len);
        void syncWriteSpd(int id[], int speed[], int acc[], int len);
    };

#endif