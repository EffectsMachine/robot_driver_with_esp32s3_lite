#include "jointsCtrl.h"

void JointsCtrl::init(int baud) {
    Serial1.begin(baud, SERIAL_8N1, BUS_SERVO_RX, BUS_SERVO_TX);
    sc.pSerial = &Serial1;
    smst.pSerial = &Serial1;
    hl.pSerial = &Serial1;
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
    } else {
        return false;
    }
}

void JointsCtrl::setEncoderStepRange(u_int16_t steps, double angle) {
    if (steps < 512) {
        return false;
    }
    if (angle < 90 || angle > 360) {
        return false;
    }
    jointSteps = steps;
    middleSteps = round(jointSteps/2 - 1);
    jointRangeAngle = angle;
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

    int feedback[SERVO_FEEDBACK_NUM];
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
            if (smst.getLasrError()) {
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

void JointsCtrl::singleSC(u_int8_t id, int pos, int time, int speed) {
    sc.WritePos(id, pos, time, speed);
}

void JointsCtrl::singleSMST(u_int8_t id, int pos, int speed, int acc) {
    smst.WritePosEx(id, pos, speed, acc);
}

void JointsCtrl::singleHL(u_int8_t id, int pos, int speed, int acc, int currt_limit) {
    hl.WritePosEx(id, pos, speed, acc, currt_limit);
}

double JointsCtrl::mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void JointsCtrl::angleCtrlSC(u_int8_t id, int mid_pos, double angle, double speed) {
    sc.WritePos(id, 
                mid_pos + round(mapDouble(angle, 0, jointRangeAngle, 0, jointSteps)), 
                0, 
                round(mapDouble(speed, 0, jointRangeAngle, 0, jointSteps)));
}

void JointsCtrl::angleCtrlSMST(u_int8_t id, double angle, double speed, double acc) {
    smst.WritePosEx(id, 
                    middleSteps + round((angle, 0, jointRangeAngle, 0, jointSteps)), 
                    round(mapDouble(speed, 0, jointRangeAngle, 0, jointSteps)), 
                    round(mapDouble(acc, 0, jointRangeAngle, 0, jointSteps)));
}

void JointsCtrl::angleCtrlHL(u_int8_t id, double angle, double speed, double acc, int currt_limit) {
    hl.WritePosEx(id, 
                  middleSteps + round((angle, 0, jointRangeAngle, 0, jointSteps)), 
                  round(mapDouble(speed, 0, jointRangeAngle, 0, jointSteps)), 
                  round(mapDouble(acc, 0, jointRangeAngle, 0, jointSteps)),
                  currt_limit)
}