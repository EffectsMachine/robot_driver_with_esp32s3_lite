/*
 * Modbus.h
 * ModbusRtu硬件接口层程序
 * 日期: 2025.04.14
 * 作者: txl
 */


#include "Modbus.h"

Modbus::Modbus()
{
	pSerial = NULL;
	txLen = 0;
}

void Modbus::begin(HardwareSerial *_pSerial)
{
	pSerial = _pSerial;
}

void Modbus::begin(HardwareSerial *_pSerial, long u32speed)
{
	pSerial = _pSerial;
	pSerial->begin(u32speed);
	txEnd_T35 = ((1000000*35)/u32speed);
}

void Modbus::begin(HardwareSerial *_pSerial, long u32speed, uint32_t config, int8_t rxPin, int8_t txPin)
{
	pSerial = _pSerial;
	pSerial->begin(u32speed, config, rxPin, txPin);
	txEnd_T35 = ((1000000*35)/u32speed);
}

void Modbus::setTimeOut(uint16_t u16timeOut)
{
	pSerial->setTimeout(u16timeOut);
}

int Modbus::readModbus(uint8_t *nDat, uint8_t nLen)
{
	return pSerial->readBytes(nDat, nLen);
}

int Modbus::writeModbus(uint8_t *nDat, uint8_t nLen)
{
	if(nDat==NULL){
		return 0;
	}
	if(isFlush){
		return pSerial->write(nDat, nLen);
	}else{
		memcpy(txBuf+txLen, nDat, nLen);
		txLen += nLen;
		return txLen;
	}
}

void Modbus::flushModbusRx()
{
	while(pSerial->read()!=-1);
}

void Modbus::flushModbusTx()
{
	if(!isFlush){
		pSerial->write(txBuf, txLen);
		txLen = 0;
	}
	pSerial->flush();
	delayMicroseconds(txEnd_T35);
}