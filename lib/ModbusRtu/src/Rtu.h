#ifndef _RTU_H
#define _RTU_H

#include "stdint.h"

/**
 * @struct modbus_t
 * @brief
 * Master query structure:
 * This includes all the necessary fields to make the Master generate a Modbus query.
 * A Master may keep several of these structures and send them cyclically or
 * use them according to program needs.
 */
typedef struct
{
    uint8_t u8id;          /*!< Slave address between 1 and 247. 0 means broadcast */
    uint8_t u8fct;         /*!< Function code: 1, 2, 3, 4, 5, 6, 15 or 16 */
    uint16_t u16RegAdd;    /*!< Address of the first register to access at slave/s */
    uint16_t u16CoilsNo;   /*!< Number of coils or registers to access */
    uint16_t *au16reg;     /*!< Pointer to memory image in master */
	uint8_t ack;
}
modbus_t;

enum
{
    RESPONSE_SIZE = 6,
    EXCEPTION_SIZE = 3,
    CHECKSUM_SIZE = 2
};

/**
 * @enum MESSAGE
 * @brief
 * Indexes to telegram frame positions
 */
enum MESSAGE
{
    ID                             = 0, //!< ID field
    FUNC, //!< Function code position
    ADD_HI, //!< Address high byte
    ADD_LO, //!< Address low byte
    NB_HI, //!< Number of coils or registers high byte
    NB_LO, //!< Number of coils or registers low byte
    BYTE_CNT  //!< byte counter
};

/**
 * @enum MB_FC
 * @brief
 * Modbus function codes summary.
 * These are the implement function codes either for Master or for Slave.
 *
 * @see also fctsupported
 * @see also modbus_t
 */
enum MB_FC
{
    MB_FC_NONE                     = 0,   /*!< null operator */
    MB_FC_READ_COILS               = 1,	/*!< FCT=1 -> read coils or digital outputs */
    MB_FC_READ_DISCRETE_INPUT      = 2,	/*!< FCT=2 -> read digital inputs */
    MB_FC_READ_REGISTERS           = 3,	/*!< FCT=3 -> read registers or analog outputs */
    MB_FC_READ_INPUT_REGISTER      = 4,	/*!< FCT=4 -> read analog inputs */
    MB_FC_WRITE_COIL               = 5,	/*!< FCT=5 -> write single coil or output */
    MB_FC_WRITE_REGISTER           = 6,	/*!< FCT=6 -> write single register */
    MB_FC_WRITE_MULTIPLE_COILS     = 15,/*!< FCT=15 -> write multiple coils or outputs */
    MB_FC_WRITE_MULTIPLE_REGISTERS = 16,/*!< FCT=16 -> write multiple registers */
    MB_FC_REPORT_ID = 17,	            /*!< FCT=17 -> report slave ID */
    MB_FC_REBOOT = 65,	                /*!< FCT=65 -> reboot */
    MB_FC_USER_WRITE = 66,	            /*!< FCT=66 -> user write */
    MB_FC_SYNC_WRITE = 67,	            /*!< FCT=67 -> sync write */
    MB_FC_SYNC_ACTION = 68,	            /*!< FCT=68 -> action */
    MB_FC_SPEED_CTL = 69,	            /*!< FCT=69 -> speed ctl */
    MB_FC_POSITION_CTL = 70,	        /*!< FCT=70 -> position ctl */
    MB_FC_TORQUE_CTL = 71,	            /*!< FCT=70 -> torque ctl */
    MB_FC_SETP_CTL = 72,	            /*!< FCT=70 -> setp ctl */
    MB_FC_STATUS_FB = 73,	            /*!< FCT=71 -> status feedback */
};

enum COM_STATES
{
    COM_IDLE       = 0,
    COM_WAITING    = 1

};

enum MB_ERR_LIST
{
    MBERR_NO_REPLY    = 1,
    MBERR_FUNC_CODE   = 2,
    MBERR_CRC_CMP     = 3,
    MBERR_SLAVE_ID    = 4,
	MBERR_BUFF_OVERFLOW = 5,

};

const unsigned char fctsupported[] =
{
    MB_FC_READ_COILS,
    MB_FC_READ_DISCRETE_INPUT,
    MB_FC_READ_REGISTERS,
    MB_FC_READ_INPUT_REGISTER,
    MB_FC_WRITE_COIL,
    MB_FC_WRITE_REGISTER,
    MB_FC_WRITE_MULTIPLE_COILS,
    MB_FC_WRITE_MULTIPLE_REGISTERS,
	MB_FC_REPORT_ID,
	MB_FC_REBOOT,
	MB_FC_USER_WRITE,
	MB_FC_SYNC_WRITE,
	MB_FC_SYNC_ACTION,
	MB_FC_SPEED_CTL,
	MB_FC_POSITION_CTL,
	MB_FC_TORQUE_CTL,
	MB_FC_SETP_CTL,
	MB_FC_STATUS_FB
};

#define  MAX_BUFFER  64	//!< maximum size for the communication buffer in bytes

/**
 * @class Modbus
 * @brief
 * RTU protocol
 */
class Rtu
{
protected:
    uint8_t u8state;
    uint8_t u8lastError;
    uint8_t au8Buffer[MAX_BUFFER];
    uint8_t u8BufferSize;
    modbus_t telegram;
	uint8_t slaveID;
	uint8_t isFlush;

	void init();
	uint8_t getRxu8Len();
    uint16_t calcCRC(uint8_t u8length);
    uint8_t validateAnswer();
    void get_FC1();
    void get_FC3();
	void get_FC4();

    Rtu();
    void query(); //!<only for master
    void poll(); //!<cyclic poll for master
protected:
	virtual void flushModbusRx() = 0;
	virtual void flushModbusTx() = 0;
	virtual int writeModbus(uint8_t *nDat, uint8_t nLen) = 0;
	virtual int readModbus(uint8_t *nDat, uint8_t nLen) = 0;

public:
	uint8_t getState() { return u8state; }
	uint8_t getLastError() { return u8lastError; }
	uint8_t getSlaveID() { return slaveID; }
	uint8_t getFlush() { return isFlush; }
	void setFlush(uint8_t _flush) { isFlush = _flush; }

	uint8_t writeReg(uint8_t ID, uint16_t regAddr, uint16_t wDat);
	uint8_t writeReg(uint8_t ID, uint16_t regAddr, uint16_t *wDat, uint8_t nLen);
	uint8_t readReg(uint8_t ID, uint16_t regAddr, uint16_t *wDat, uint8_t nLen);
	int16_t readReg(uint8_t ID, uint16_t regAddr);
	int Ping(uint8_t ID);
	void reBoot(uint8_t ID);
	void writeUser(uint8_t ID, uint16_t regAddr, uint16_t wDat);
	void writeUser(uint8_t ID, uint16_t regAddr, uint16_t *wDat, uint8_t nLen);
	void writeSync(uint8_t ID, uint16_t regAddr, uint16_t wDat);
	void writeSync(uint8_t ID, uint16_t regAddr, uint16_t *wDat, uint8_t nLen);
	void actionSync(uint8_t ID);
	void flushTx() { flushModbusTx(); }
};

#endif