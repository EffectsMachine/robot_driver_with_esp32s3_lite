/*
 * GQDmd.h
 * GJW驱动应用层程序
 * 日期: 2025.04.14
 * 作者: txl
 */

#include "GQDmd.h"

GQDMD::GQDMD()
{}

//舵机ID，Position位置
void GQDMD::SyncWritePos(uint8_t ID, int32_t Position)
{
	uint16_t wDat[2];
	wDat[0] = Position>>16;
	wDat[1] = Position&0xffff;
	writeSync(ID, GQDMD_GOAL_POSITION, wDat, 2);
}


//模式切换
void GQDMD::WheelMode(uint8_t ID)
{
	int cf = 0;
	int Mode = readReg(ID, GQDMD_MODE);
	if(!getLastError()){
		if(Mode==1){
			cf = 1;
		}
	}
	if(!cf){
		WriteMode(ID, 1);
	}
}

void GQDMD::ServoMode(uint8_t ID)
{
	int cf = 0;
	int Mode = readReg(ID, GQDMD_MODE);
	if(!getLastError()){
		if(Mode==0){
			cf = 1;
		}
	}
	if(!cf){
		WriteMode(ID, 0);
	}
}

int GQDMD::WriteMode(uint8_t ID, uint16_t Mode)
{
	unLockMem(ID, GQDMD_MODE, 0);
	writeReg(ID, GQDMD_MODE, Mode);	
	return LockMem(ID);
}

//扭力控制指令
int GQDMD::EnableTorque(uint8_t ID, uint8_t Enable)
{
	return writeReg(ID, GQDMD_TORQUE_ENABLE, Enable);
}

//eprom解锁
int GQDMD::unLockMem(uint8_t ID, uint16_t wAddr, uint16_t Key)
{
  uint16_t lkDat[3];
  lkDat[0] = 0;
  lkDat[1] = wAddr/64;
  lkDat[2] = Key;
  return writeReg(ID, GQDMD_TORQUE_ENABLE, lkDat, 3);
}

//关闭EPROM保存功能
int GQDMD::LockMem(uint8_t ID)
{
  uint16_t lkDat[3];
  lkDat[0] = 0;
  lkDat[1] = 0;
  lkDat[2] = 0;
  return writeReg(ID, GQDMD_TORQUE_ENABLE, lkDat, 3);
}

int GQDMD::CalibrationOfs(uint8_t ID)
{
	unLockMem(ID, 4*64, 0);
	writeReg(ID, GQDMD_FAULT_RESET, GQDMD_FAULT_OFS);
	return LockMem(ID);
}

int GQDMD::SpeedCtl(int ID, int16_t Speed, int16_t ACC, int16_t Torque, int16_t beatTime, uint8_t Ack)
{
	uint16_t wDat[6];
	telegram.u8id = ID;
	telegram.u8fct = MB_FC_SPEED_CTL;
    telegram.u16CoilsNo = 4;
    telegram.au16reg = wDat;
	telegram.ack = Ack;
	wDat[0] = Speed;
	wDat[1] = ACC;
	wDat[2] = Torque;
	wDat[3] = beatTime;
    query();
	if(telegram.ack&MD_CTL_ACK){
		while(u8state != COM_IDLE){
			poll();
		}
		if(!u8lastError){
			motorTurn = (wDat[1]|(wDat[0]<<16));
			motorAngle = wDat[2];
			motorSpeed = wDat[3];
			motorTorque = wDat[4];
			motorStatus = wDat[5];
			return 1;
		}else{
			return 0;
		}
	}else{
		u8state = COM_IDLE;
	}
	return 1;
}

int GQDMD::PositionCtl(int ID, int32_t Position, int16_t Speed, int16_t ACC, int16_t Torque, uint8_t Ack)
{
	uint16_t wDat[6];
	telegram.u8id = ID;
	telegram.u8fct = MB_FC_POSITION_CTL;
    telegram.u16CoilsNo = 5;
    telegram.au16reg = wDat;
	telegram.ack = Ack;
	wDat[0] = Position>>16;
	wDat[1] = Position&0xffff;
	wDat[2] = Speed;
	wDat[3] = ACC;
	wDat[4] = Torque;
    query();
	if(telegram.ack&MD_CTL_ACK){
		while(u8state != COM_IDLE){
			poll();
		}
		if(!u8lastError){
			motorTurn = (wDat[1]|(wDat[0]<<16));
			motorAngle = wDat[2];
			motorSpeed = wDat[3];
			motorTorque = wDat[4];
			motorStatus = wDat[5];
			return 1;
		}else{
			return 0;
		}
	}else{
		u8state = COM_IDLE;
	}
	return 1;
}

int GQDMD::TorqueCtl(int ID, int16_t Torque, int16_t beatTime, uint8_t Ack)
{
	uint16_t wDat[6];
	telegram.u8id = ID;
	telegram.u8fct = MB_FC_TORQUE_CTL;
    telegram.u16CoilsNo = 2;
    telegram.au16reg = wDat;
	telegram.ack = Ack;
	wDat[0] = Torque;
	wDat[1] = beatTime;
    query();
	if(telegram.ack&MD_CTL_ACK){
		while(u8state != COM_IDLE){
			poll();
		}
		if(!u8lastError){
			motorTurn = (wDat[1]|(wDat[0]<<16));
			motorAngle = wDat[2];
			motorSpeed = wDat[3];
			motorTorque = wDat[4];
			motorStatus = wDat[5];
			return 1;
		}else{
			return 0;
		}
	}else{
		u8state = COM_IDLE;
	}
	return 1;
}

int GQDMD::SetpCtl(int ID, int32_t Setp, int16_t Speed, int16_t ACC, int16_t Torque, uint8_t Ack)
{
	uint16_t wDat[6];
	telegram.u8id = ID;
	telegram.u8fct = MB_FC_SETP_CTL;
    telegram.u16CoilsNo = 5;
    telegram.au16reg = wDat;
	telegram.ack = Ack;
	wDat[0] = Setp>>16;
	wDat[1] = Setp&0xffff;
	wDat[2] = Speed;
	wDat[3] = ACC;
	wDat[4] = Torque;
    query();
	if(telegram.ack&MD_CTL_ACK){
		while(u8state != COM_IDLE){
			poll();
		}
		if(!u8lastError){
			motorTurn = (wDat[1]|(wDat[0]<<16));
			motorAngle = wDat[2];
			motorSpeed = wDat[3];
			motorTorque = wDat[4];
			motorStatus = wDat[5];
			return 1;
		}else{
			return 0;
		}
	}else{
		u8state = COM_IDLE;
	}
	return 1;
}

int GQDMD::StatusCtl(int ID, uint16_t rstErr)
{
	uint16_t wDat[6];
	telegram.u8id = ID;
	telegram.u8fct = MB_FC_STATUS_FB;
    telegram.u16CoilsNo = 1;
    telegram.au16reg = wDat;
	wDat[0] = rstErr;
    query();
	while(u8state != COM_IDLE){
		poll();
	}
	if(!u8lastError&MD_CTL_ACK){
		motorTurn = (wDat[1]|(wDat[0]<<16));
		motorAngle = wDat[2];
		motorSpeed = wDat[3];
		motorTorque = wDat[4];
		motorStatus = wDat[5];
		return 1;
	}else{
		return 0;
	}
}