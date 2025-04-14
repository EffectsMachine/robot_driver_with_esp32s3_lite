#include "jointsCtrl.h"
#include "Config.h"

void JointsCtrl::init(int baud) {
    Serial1.begin(baud, SERIAL_8N1, BUS_SERVO_RX, BUS_SERVO_TX);
    sc.pSerial = &Serial1;
    smst.pSerial = &Serial1;
    hl.pSerial = &Serial1;

    gqdmd.begin(&Serial1);
    gqdmd.setTxEnd_T32(1000000);
    gqdmd.setTimeOut(2);

    while(!Serial1) {}
}

void JointsCtrl::setBaudRate(int baud) {
    Serial1.updateBaudRate(baud);
    // sc.updateBaudRate(baud);
    // sms_sts.updateBaudRate(baud);
}

bool JointsCtrl::setJointType(u_int8_t type) {
    switch (type) {
        case JOINT_TYPE_SC:
            jointType = type;
            return true;
            break;
        case JOINT_TYPE_SMST:
            jointType = type;
            return true;
            break;
        case JOINT_TYPE_HL:
            jointType = type;
            return true;
            break;
    } 
    return false;
}

bool JointsCtrl::setEncoderStepRange(u_int16_t steps, double angle) {
    if (steps < 512) {
        return false;
    }
    if (angle < 90 || angle > 360) {
        return false;
    }
    jointSteps = steps;
    middleSteps = round(jointSteps/2 - 1);
    jointRangeAngle = angle;
    jointRangeRad = jointRangeAngle * M_PI / 180.0;

    for(int i = 0; i < JOINTS_NUM; i++) {
        jointsZeroPos[i] = middleSteps; // or any other default value
        jointsFeedbackPos[i] = middleSteps; // or any other default value
        jointsCurrentPos[i] = middleSteps; // or any other default value
        jointsGoalPos[i] = middleSteps; // or any other default value
        jointsLastPos[i] = middleSteps; // or any other default value
    }

    return true;
}

int* JointsCtrl::singleFeedBack(u_int8_t id) {
    // Get feedback from servo
    // [0]ping status
    // [1]position
    // [2]speed
    // [3]load
    // [4]voltage
    // [5]temperature
    // [6]moving
    // [7]current

    static int feedback[SERVO_FEEDBACK_NUM];
    switch (jointType) {
        case JOINT_TYPE_SC:
            sc.FeedBack(id);
            if (sc.getLastError()) {
                feedback[FB_PING] = -1;
                break;
            }
            feedback[FB_PING] = 1;
            feedback[FB_POS] = sc.ReadPos(-1);
            feedback[FB_SPD] = sc.ReadSpeed(-1);
            feedback[FB_LOAD] = sc.ReadLoad(-1);
            feedback[FB_VOLT] = sc.ReadVoltage(-1);
            feedback[FB_TEMP] = sc.ReadTemper(-1);
            feedback[FB_MOVE] = sc.ReadMove(-1);
            feedback[FB_CURT] = sc.ReadCurrent(-1);
            break;
        case JOINT_TYPE_SMST:
            smst.FeedBack(id);
            if (smst.getLastError()) {
                feedback[FB_PING] = -1;
                break;
            }
            feedback[FB_PING] = 1;
            feedback[FB_POS] = smst.ReadPos(-1);
            feedback[FB_SPD] = smst.ReadSpeed(-1);
            feedback[FB_LOAD] = smst.ReadLoad(-1);
            feedback[FB_VOLT] = smst.ReadVoltage(-1);
            feedback[FB_TEMP] = smst.ReadTemper(-1);
            feedback[FB_MOVE] = smst.ReadMove(-1);
            feedback[FB_CURT] = smst.ReadCurrent(-1);
            break;
        case JOINT_TYPE_HL:
            hl.FeedBack(id);
            if (hl.getLastError()) {
                feedback[FB_PING] = -1;
                break;
            }
            feedback[FB_PING] = 1;
            feedback[FB_POS] = hl.ReadPos(-1);
            feedback[FB_SPD] = hl.ReadSpeed(-1);
            feedback[FB_LOAD] = hl.ReadLoad(-1);
            feedback[FB_VOLT] = hl.ReadVoltage(-1);
            feedback[FB_TEMP] = hl.ReadTemper(-1);
            feedback[FB_MOVE] = hl.ReadMove(-1);
            feedback[FB_CURT] = hl.ReadCurrent(-1);
            break;
    }

    return feedback;
}

bool JointsCtrl::ping(u_int8_t id) {
    switch (jointType) {
        case JOINT_TYPE_SC:
            sc.Ping(id);
            if (sc.getLastError()) {
                return false;
            } else {
                return true;
            }
            break;
        case JOINT_TYPE_SMST:
            smst.Ping(id);
            if (smst.getLastError()) {
                return false;
            } else {
                return true;
            }
            break;
        case JOINT_TYPE_HL:
            hl.Ping(id);
            if (hl.getLastError()) {
                return false;
            } else {
                return true;
            }
            break;
    }
    return false;
}

bool JointsCtrl::changeID(u_int8_t old_id, u_int8_t new_id) {
    switch (jointType) {
        case JOINT_TYPE_SC:
            if (!ping(old_id)) {
                return false;
            } else {
                sc.unLockEprom(old_id);
                sc.writeByte(old_id, SCSCL_ID, new_id); // change address
                sc.LockEprom(new_id);
                return true;
            }
            break;
        case JOINT_TYPE_SMST:
            if (!ping(old_id)) {
                return false;
            } else {
                smst.unLockEprom(old_id);
                smst.writeByte(old_id, SMS_STS_ID, new_id);
                smst.LockEprom(new_id);
                return true;
            }
            break;
        case JOINT_TYPE_HL:
            if (!ping(old_id)) {
                return false;
            } else {
                hl.unLockEprom(old_id);
                hl.writeByte(old_id, SMS_STS_ID, new_id); // change address
                hl.LockEprom(new_id);
                return true;
            }
            break;
    }
    return false;
}

bool JointsCtrl::setMiddle(u_int8_t id) {
    switch (jointType) {
        case JOINT_TYPE_SC:
            return false;
            break;
        case JOINT_TYPE_SMST:
            if (!ping(id)) {
                return false;
            } else {
                smst.CalibrationOfs(id);
                return true;
            }
            break;
        case JOINT_TYPE_HL:
            if (!ping(id)) {
                return false;
            } else {
                hl.CalibrationOfs(id);
                return true;
            }
            break;
    }
    return false;
}

void JointsCtrl::moveMiddle(u_int8_t id) {
    switch (jointType) {
        case JOINT_TYPE_SC:
            sc.WritePos(id, middleSteps, 0, 300);
            break;
        case JOINT_TYPE_SMST:
            smst.WritePosEx(id, middleSteps, 300, 50);
            break;
        case JOINT_TYPE_HL:
            hl.WritePosEx(id, middleSteps, 10, 50, 500);
            break;
    }
}

void JointsCtrl::torqueLock(u_int8_t id, bool state) {
    switch (jointType) {
        case JOINT_TYPE_SC:
            if (state) {
                sc.EnableTorque(id, 1);
            } else {
                sc.EnableTorque(id, 0);
            }
            break;
        case JOINT_TYPE_SMST:
            if (state) {
                smst.EnableTorque(id, 1);
            } else {
                smst.EnableTorque(id, 0);
            }
            break;
        case JOINT_TYPE_HL:
            if (state) {
                hl.EnableTorque(id, 1);
            } else {
                hl.EnableTorque(id, 0);
            }
            break;
    }
}

void JointsCtrl::stepsCtrlSC(u_int8_t id, int pos, int time, int speed, bool move_trigger) {
    if (move_trigger) {
        sc.WritePos(id, pos, time, speed);
    } else {
        sc.RegWritePos(id, pos, time, speed);
    }
}

void JointsCtrl::stepsCtrlSMST(u_int8_t id, int pos, int speed, int acc, bool move_trigger) {
    if (move_trigger) {
        smst.WritePosEx(id, pos, speed, acc);
    } else {
        smst.RegWritePosEx(id, pos, speed, acc);
    }
}

void JointsCtrl::stepsCtrlHL(u_int8_t id, int pos, int speed, int acc, int currt_limit, bool move_trigger) {
    if (move_trigger) {
        hl.WritePosEx(id, pos, speed, acc, currt_limit);
    } else {
        hl.RegWritePosEx(id, pos, speed, acc, currt_limit);
    }
}

double JointsCtrl::mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int JointsCtrl::angleCtrlSC(u_int8_t id, int mid_pos, double angle, double speed, bool move_trigger) {
    int goal_pos = mid_pos + round(mapDouble(angle, 0, jointRangeAngle, 0, jointSteps));
    int goal_spd = round(mapDouble(speed, 0, jointRangeAngle, 0, jointSteps));
    if (move_trigger) {
        sc.WritePos(id, goal_pos, 0, goal_spd);
    } else {
        sc.RegWritePos(id, goal_pos, 0, goal_spd);
    }
    return goal_pos;
}

int JointsCtrl::angleCtrlSMST(u_int8_t id, double angle, double speed, double acc, bool move_trigger) {
    int goal_pos = middleSteps + round(mapDouble(angle, 0, jointRangeAngle, 0, jointSteps));
    int goal_spd = round(mapDouble(speed, 0, jointRangeAngle, 0, jointSteps));
    int goal_acc = round(mapDouble(acc, 0, jointRangeAngle, 0, jointSteps));
    if (move_trigger) {
        smst.WritePosEx(id, goal_pos, goal_spd, goal_acc);
    } else {
        smst.RegWritePosEx(id, goal_pos, goal_spd, goal_acc);
    }
    return goal_pos;
}

int JointsCtrl::angleCtrlHL(u_int8_t id, double angle, double speed, double acc, int currt_limit, bool move_trigger) {
    int goal_pos = middleSteps + round(mapDouble(angle, 0, jointRangeAngle, 0, jointSteps));
    int goal_spd = round(mapDouble(speed, 0, jointRangeAngle, 0, jointSteps));
    int goal_acc = round(mapDouble(acc, 0, jointRangeAngle, 0, jointSteps));
    if (move_trigger) {
        hl.WritePosEx(id, goal_pos, goal_spd, goal_acc, currt_limit);
    } else {
        hl.RegWritePosEx(id, goal_pos, goal_spd, goal_acc, currt_limit);
    }
    return goal_pos;
}

int JointsCtrl::radCtrlSC(u_int8_t id, int mid_pos, double rad, double speed, bool move_trigger) {
    int goal_pos = mid_pos + round(mapDouble(rad, 0, jointRangeRad, 0, jointSteps));
    int goal_spd = round(mapDouble(speed, 0, jointRangeRad, 0, jointSteps));
    if (move_trigger) {
        sc.WritePos(id, goal_pos, 0, goal_spd);
    } else {
        sc.RegWritePos(id, goal_pos, 0, goal_spd);
    }
    return goal_pos;
}

int JointsCtrl::radCtrlSMST(u_int8_t id, double rad, double speed, double acc, bool move_trigger) {
    int goal_pos = middleSteps + round(mapDouble(rad, 0, jointRangeRad, 0, jointSteps));
    int goal_spd = round(mapDouble(speed, 0, jointRangeRad, 0, jointSteps));
    int goal_acc = round(mapDouble(acc, 0, jointRangeRad, 0, jointSteps));
    if (move_trigger) {
        smst.WritePosEx(id, goal_pos, goal_spd, goal_acc);
    } else {
        smst.RegWritePosEx(id, goal_pos, goal_spd, goal_acc);
    }
    return goal_pos;
}

int JointsCtrl::radCtrlHL(u_int8_t id, double rad, double speed, double acc, int currt_limit, bool move_trigger) {
    int goal_pos = middleSteps + round(mapDouble(rad, 0, jointRangeRad, 0, jointSteps));
    int goal_spd = round(mapDouble(speed, 0, jointRangeRad, 0, jointSteps));
    int goal_acc = round(mapDouble(acc, 0, jointRangeRad, 0, jointSteps));
    if (move_trigger) {
        hl.WritePosEx(id, goal_pos, goal_spd, goal_acc, currt_limit);
    } else {
        hl.RegWritePosEx(id, goal_pos, goal_spd, goal_acc, currt_limit);
    }
    return goal_pos;
}

void JointsCtrl::moveTrigger() {
    switch (jointType) {
        case JOINT_TYPE_SC:
            sc.RegWriteAction();
            break;
        case JOINT_TYPE_SMST:
            smst.RegWriteAction();
            break;
        case JOINT_TYPE_HL:
            hl.RegWriteAction();
            break;
    }
}

// hub motor ctrl
void JointsCtrl::hubMotorCtrl(int spd_1, int spd_2, int spd_3, int spd_4) {
    gqdmd.SpeedCtl(1, spd_1, 500, 600, 0);
    gqdmd.SpeedCtl(2, spd_2, 500, 600, 0);
    gqdmd.SpeedCtl(3, spd_3, 500, 600, 0);
    gqdmd.SpeedCtl(4, spd_4, 500, 600, 0);
}

// for applications: LyLinkArm
int* JointsCtrl::getJointsZeroPosArray() {
    return jointsZeroPos;
}

void JointsCtrl::setJointsZeroPosArray(int values[]) {
    for (int i = 0; i < JOINTS_NUM; i++) {
        jointsZeroPos[i] = values[i];
    }
}

int* JointsCtrl::getLinkArmPosSC() {
    for (int i = 0; i < JOINTS_NUM; i++) {
        jointsFeedbackPos[i] = sc.ReadPos(jointID[i]);
    }
    return jointsFeedbackPos;
}

void JointsCtrl::setCurrentSCPosMiddle() {
    for (int i = 0; i < JOINTS_NUM; i++) {
        jointsFeedbackPos[i] = sc.ReadPos(jointID[i]);
        jointsZeroPos[i] = jointsFeedbackPos[i];
    }
}

// for applications: LyLinkArm
// joint_1 : as the base joint
// joint_2 : as the shoulder joint (base front)
// joint_3 : as the elbow joint (base back)
// joint_4 : as the gripper joint
// >-------O----O
// |       |   /
// 3       |  /
//       1-S E-2
//          B-0
void JointsCtrl::linkArmSCJointsCtrlAngle(double angles[]) {
    for (int i = 0; i < JOINTS_NUM; i++) {
        jointsCurrentPos[i] = angleCtrlSC(jointID[i], jointsZeroPos[i], angles[i], 0, false);
    }
    moveTrigger();
}

void JointsCtrl::linkArmSCJointsCtrlRad(double rads[]) {
    for (int i = 0; i < JOINTS_NUM; i++) {
        jointsCurrentPos[i] = radCtrlSC(jointID[i], jointsZeroPos[i], rads[i], 0, false);
    }
    moveTrigger();
}