#ifndef JOINTS_CTRL_H
#define JOINTS_CTRL_H

#include <SCServo.h>
#include <GJWMotor.h>

#define JOINT_TYPE_SC 0
#define JOINT_TYPE_SMST 1
#define JOINT_TYPE_HL 2

#define JOINTS_NUM 4 // Number of joints
#define JOINTS_SC_MAX_POS 1023 // Maximum position for each joint in steps
#define JOINTS_SMST_MAX_POS 4095 // Maximum position for each joint in steps
#define JOINTS_HL_MAX_POS 4095 // Maximum position for each joint in steps
#define JOINTS_SC_RANGE_ANGLE 360.0 // Maximum angle for each joint in degrees
#define JOINTS_SMST_RANGE_ANGLE 360.0 // Maximum angle for each joint in degrees
#define JOINTS_HL_RANGE_ANGLE 360.0 // Maximum angle for each joint in degrees

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
        SCSCL sc;
        SMS_STS smst;
        HLSCL hl;
        GQDMD gqdmd;

        // for applications: LyLinkArm
        int jointsZeroPos[JOINTS_NUM]; // array to store the zero position of each joint
        int jointsFeedbackPos[JOINTS_NUM]; // array to store the feedback position from each joint
        int jointsCurrentPos[JOINTS_NUM]; // array to store the current position of each joint
        int jointsGoalPos[JOINTS_NUM]; // array to store the goal position of each joint
        int jointsLastPos[JOINTS_NUM]; // array to store the last position of each joint
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
        u_int16_t jointSteps = JOINTS_SMST_MAX_POS; // steps in one circle
        u_int16_t middleSteps = jointSteps/2 - 1;
        double jointRangeAngle = JOINTS_SMST_RANGE_ANGLE;
        double jointRangeRad = jointRangeAngle * M_PI / 180.0; // wiggle range in radian

    public:
        // for applications: LyLinkArm
        JointsCtrl() {
            // Initialize jointsZeroPos array
            for(int i = 0; i < JOINTS_NUM; i++) {
                jointsZeroPos[i] = middleSteps; // or any other default value
                jointsFeedbackPos[i] = middleSteps; // or any other default value
                jointsCurrentPos[i] = middleSteps; // or any other default value
                jointsGoalPos[i] = middleSteps; // or any other default value
                jointsLastPos[i] = middleSteps; // or any other default value
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

        void stepsCtrlSC(u_int8_t id, int pos, int time, int speed, bool move_trigger = true);
        void stepsCtrlSMST(u_int8_t id, int pos, int speed, int acc, bool move_trigger = true);
        void stepsCtrlHL(u_int8_t id, int pos, int speed, int acc, int currt_limit, bool move_trigger = true);
        double mapDouble(double x, double in_min, double in_max, double out_min, double out_max);
        int angleCtrlSC(u_int8_t id, int mid_pos, double angle, double speed, bool move_trigger = true);
        int angleCtrlSMST(u_int8_t id, double angle, double speed, double acc, bool move_trigger = true);
        int angleCtrlHL(u_int8_t id, double angle, double speed, double acc, int currt_limit, bool move_trigger = true);
        int radCtrlSC(u_int8_t id, int mid_pos, double rad, double speed, bool move_trigger = true);
        int radCtrlSMST(u_int8_t id, double rad, double speed, double acc, bool move_trigger = true);
        int radCtrlHL(u_int8_t id, double rad, double speed, double acc, int currt_limit, bool move_trigger = true);
        void moveTrigger();

        // hub motor ctrl
        void hubMotorCtrl(int spd_1, int spd_2, int spd_3, int spd_4);

        // for applications: LyLinkArm
        int* getJointsZeroPosArray();
        void setJointsZeroPosArray(int values[]);
        int* getLinkArmPosSC();
        void setCurrentSCPosMiddle();
        void linkArmSCJointsCtrlAngle(double angles[]);
        void linkArmSCJointsCtrlRad(double rads[]);
        
    };

#endif