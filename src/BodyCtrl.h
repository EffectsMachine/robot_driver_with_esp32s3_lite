#ifndef BODY_CTRL_H
#define BODY_CTRL_H

#include <SCServo.h>

class BodyCtrl{
private:
    SCSCL sc;
    int jointsZeroPos[12]; // array to store the zero position of each joint
    int jointsCurrentPos[12]; // array to store the current position of each joint
    int jointsGoalPos[12]; // array to store the goal position of each joint
    int directionArray[12]; // array to store the direction of each joint
    int jointID[12] = {52, 53, 51, 42, 43, 41, 22, 23, 21, 32, 33, 31};
    // [0]ping status
    // [1]position
    // [2]speed
    // [3]load
    // [4]voltage
    // [5]temperature
    // [6]moving
    // [7]current
    int servoFeedback[0];

public:
    BodyCtrl() {
        // Initialize jointsZeroPos array
        for(int i = 0; i < 12; i++) {
            jointsZeroPos[i] = 512; // or any other default value
            jointsCurrentPos[i] = 512; // or any other default value
            jointsGoalPos[i] = 512; // or any other default value
        }
        directionArray[0] = -1;
        directionArray[1] = 1; 
        directionArray[2] = -1;
        directionArray[3] = 1;
        directionArray[4] = -1;
        directionArray[5] = 1;
        directionArray[6] = 1;
        directionArray[7] = -1;
        directionArray[8] = 1;
        directionArray[9] = -1;
        directionArray[10] = 1;
        directionArray[11] = -1;
    }
    void init();
    void jointMiddle();
    void releaseTorque();
    void singleServoCtrl(int id, int pos, int noidea, int speed);
    int* getJointsZeroPosArray();
    void setJointsZeroPosArray(int values[]);
    int* getServoFeedback();
    void setCurrentPosZero();

    void jointAngle(int joint, double angleW);
    double mapDouble(double x, double in_min, double in_max, double out_min, double out_max);
    void jointRad(int joint, double rad);
    void moveTrigger();
    void stand();

    
    // servo config
    void setBaudRate(int baud);

    // SMS/ST Servo Ctrl
    void changeID(int oldID, int newID);
    void setMiddle(int id);
    int* feedBack(int id);
    void torqueLock(int id, int state);
    void wheelMode(int id);
    void servoMode(int id);
    void writePos(int id, int pos, int speed, int acc);
    void writeSpd(int id, int speed, int acc);
    void regWritePos(int id, int pos, int speed, int acc);
    void regMoveTrigger();
    void syncWritePos(int id[], int pos[], int speed[], int acc[], int len);
    void syncWriteSpd(int id[], int speed[], int acc[], int len);
};

#endif // BODYCTRL_H