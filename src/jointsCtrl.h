#ifndef JOINTS_CTRL_H
#define JOINTS_CTRL_H

#include <SCServo.h>

#define JOINT_TYPE_SC 0
#define JOINT_TYPE_SMST 1
#define JOINT_TYPE_HL 2

#define JOINTS_NUM 4 // Number of joints
#define JOINTS_SC_ZERO_POS 512 // Default zero position for each joint
#define JOINTS_SC_MAX_POS 1023 // Maximum position for each joint

#define SERVO_FEEDBACK_NUM 8 // Number of feedback parameters
#define FB_PING 0
#define FB_POS  1
#define FB_SPD  2
#define FB_LOAD 3
#define FB_VOLT 4
#define FB_TEMP 5
#define FB_MOVE 6
#define FB_CURT 7

// for applications: LyLinkArm
#define SERVO_ARRAY_0 31
#define SERVO_ARRAY_1 32
#define SERVO_ARRAY_2 33
#define SERVO_ARRAY_3 34

class JointsCtrl {
    private:
        SCSCL scs;
        SMS_STS smst;
        HLSCL hl;

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
        int servoFeedback[SERVO_FEEDBACK_NUM]; // array to store the feedback from each servo

        u_int8_t jointType = JOINT_TYPE_SMST; // JOINT_TYPE_SC, JOINT_TYPE_SMST, JOINT_TYPE_HL
        u_int16_t jointSteps = 4096; // steps in one circle
        u_int16_t middleSteps = jointSteps/2 - 1;
        double jointRangeAngle = 360.0; // wiggle range in angle

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
        bool setJointType(u_int8_t type);
        bool setEncoderStepRange(u_int16_t steps, double angle);
        int* singleFeedBack(u_int8_t id);
        bool ping(u_int8_t id);
        bool changeID(u_int8_t old_id, u_int8_t new_id);
        bool setMiddle(u_int8_t id); // sc servo can not setMiddle pos

        void moveMiddle(u_int8_t id);
        void torqueLock(u_int8_t id, bool state);
        void stepsCtrlSC(u_int8_t id, int pos, int time, int speed);
        void stepsCtrlSMST(u_int8_t id, int pos, int speed, int acc);
        void stepsCtrlHL(u_int8_t id, int pos, int speed, int acc, int currt_limit);
        double mapDouble(double x, double in_min, double in_max, double out_min, double out_max);
        void angleCtrlSC(u_int8_t id, int mid_pos, double angle, double speed);
        void angleCtrlSMST(u_int8_t id, double angle, double speed, double acc);
        void angleCtrlHL(u_int8_t id, double angle, double speed, double acc, int currt_limit);

        int* getJointsZeroPosArray();
        void setJointsZeroPosArray(int values[]);
        int* getServoFeedback();
        void setCurrentPosZero();
        
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