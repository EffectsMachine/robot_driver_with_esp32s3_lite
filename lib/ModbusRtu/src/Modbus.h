/*
 * Modbus.h
 * ModbusRtu硬件接口层程序
 * 日期: 2025.04.14
 * 作者: txl
 */

#ifndef _MODBUS_H
#define _MODBUS_H

#include "Arduino.h"
#include "Rtu.h"


class Modbus : public Rtu
{
public:
	Modbus();
	void begin(HardwareSerial *_pSerial);
	void begin(HardwareSerial *_pSerial, long u32speed);
	void begin(HardwareSerial *_pSerial, long u32speed, uint32_t config, int8_t rxPin, int8_t txPin);
	void setTimeOut(uint16_t u16timeOut);
	void setTxEnd_T32(long u32speed) {txEnd_T35 = ((1000000*35)/u32speed);}
protected:
	virtual int writeModbus(uint8_t *nDat, uint8_t nLen);//输出nLen字节
	virtual int readModbus(uint8_t *nDat, uint8_t nLen);//输入nLen字节
	virtual void flushModbusRx();//刷新接收缓冲区
	virtual void flushModbusTx();//刷新发送缓冲区
public:
	HardwareSerial *pSerial;//串口指针
    uint32_t txEnd_T35;
	uint8_t txBuf[256];
	uint8_t txLen;
};

#endif