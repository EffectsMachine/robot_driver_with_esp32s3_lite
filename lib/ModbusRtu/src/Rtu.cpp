#include <Rtu.h>

#define  lowByte(w)  ((w) & 0xff)//取低字节
#define  highByte(w)  ((w) >> 8)//取高字节
#define  word(bH, bL)  ((((uint16_t)(bH))<<8) | (uint16_t)(bL))


/**
 * Default Constructor for Master through Serial
 *
 */
Rtu::Rtu()
{
	slaveID = 0;
	u8state = 0;
	isFlush = 1;
}

/**
 * Generate a query to an slave with a modbus_t telegram structure
 * The Master must be in COM_IDLE mode. After it, its state would be COM_WAITING..
 *
 * @see modbus_t
 * @param modbus_t  modbus telegram structure (id, fct, ...)
 * @todo finish function 15
 */
void Rtu::query()
{
    uint8_t u8regsno, u8bytesno;
    if(u8state != COM_IDLE) return;

	if(telegram.u8id>247){
		u8lastError = MBERR_SLAVE_ID;
		return;
	}

    uint16_t *au16regs = telegram.au16reg;
	u8BufferSize = 0;

    // telegram header
    au8Buffer[ID]         = telegram.u8id;
    au8Buffer[FUNC]       = telegram.u8fct;

    switch(telegram.u8fct)
    {
    case MB_FC_READ_COILS:
    case MB_FC_READ_DISCRETE_INPUT:
    case MB_FC_READ_REGISTERS:
    case MB_FC_READ_INPUT_REGISTER:
		au8Buffer[ADD_HI]     = highByte(telegram.u16RegAdd);
		au8Buffer[ADD_LO]     = lowByte(telegram.u16RegAdd);
        au8Buffer[NB_HI]      = highByte(telegram.u16CoilsNo);
        au8Buffer[NB_LO]      = lowByte(telegram.u16CoilsNo);
        u8BufferSize = 6;
        break;
    case MB_FC_WRITE_COIL:
		au8Buffer[ADD_HI]     = highByte(telegram.u16RegAdd);
		au8Buffer[ADD_LO]     = lowByte(telegram.u16RegAdd);
        au8Buffer[NB_HI]      = ((au16regs[0] > 0) ? 0xff : 0);
        au8Buffer[NB_LO]      = 0;
        u8BufferSize = 6;
        break;
    case MB_FC_WRITE_REGISTER:
		au8Buffer[ADD_HI]     = highByte(telegram.u16RegAdd);
		au8Buffer[ADD_LO]     = lowByte(telegram.u16RegAdd);
        au8Buffer[NB_HI]      = highByte(au16regs[0]);
        au8Buffer[NB_LO]      = lowByte(au16regs[0]);
        u8BufferSize = 6;
        break;
    case MB_FC_WRITE_MULTIPLE_COILS: // TODO: implement "sending coils"
		au8Buffer[ADD_HI]     = highByte(telegram.u16RegAdd);
		au8Buffer[ADD_LO]     = lowByte(telegram.u16RegAdd);
        u8regsno = telegram.u16CoilsNo / 16;
        u8bytesno = u8regsno * 2;
        if((telegram.u16CoilsNo % 16) != 0){
            u8bytesno++;
            u8regsno++;
        }

        au8Buffer[NB_HI]      = highByte(telegram.u16CoilsNo);
        au8Buffer[NB_LO]      = lowByte(telegram.u16CoilsNo);
        au8Buffer[BYTE_CNT]   = u8bytesno;
        u8BufferSize = 7;

        for(uint16_t i = 0; i < u8bytesno; i++){
			if(u8BufferSize>=MAX_BUFFER){
				u8lastError = MBERR_BUFF_OVERFLOW;
				return;
			}
            if(i%2){
                au8Buffer[u8BufferSize] = lowByte(au16regs[ i/2 ]);
            }else{
                au8Buffer[u8BufferSize] = highByte(au16regs[ i/2]);
            }          
            u8BufferSize++;
        }
        break;

    case MB_FC_WRITE_MULTIPLE_REGISTERS:
		au8Buffer[ADD_HI]     = highByte(telegram.u16RegAdd);
		au8Buffer[ADD_LO]     = lowByte(telegram.u16RegAdd);
        au8Buffer[NB_HI]      = highByte(telegram.u16CoilsNo);
        au8Buffer[NB_LO]      = lowByte(telegram.u16CoilsNo);
        au8Buffer[BYTE_CNT]    = (uint8_t)(telegram.u16CoilsNo * 2);
        u8BufferSize = 7;

        for(uint16_t i=0; i< telegram.u16CoilsNo; i++){
			if((u8BufferSize+1)>=MAX_BUFFER){
				u8lastError = MBERR_BUFF_OVERFLOW;
				return;
			}
            au8Buffer[u8BufferSize] = highByte(au16regs[i]);
            u8BufferSize++;
            au8Buffer[u8BufferSize] = lowByte(au16regs[i]);
            u8BufferSize++;
        }
        break;
    case MB_FC_USER_WRITE:
	case MB_FC_SYNC_WRITE:
		au8Buffer[ADD_HI]     = highByte(telegram.u16RegAdd);
		au8Buffer[ADD_LO]     = lowByte(telegram.u16RegAdd);
        au8Buffer[NB_HI]      = highByte(telegram.u16CoilsNo);
        au8Buffer[NB_LO]      = lowByte(telegram.u16CoilsNo);
        u8BufferSize = 6;

        for(uint16_t i=0; i< telegram.u16CoilsNo; i++){
			if((u8BufferSize+1)>=MAX_BUFFER){
				u8lastError = MBERR_BUFF_OVERFLOW;
				return;
			}
            au8Buffer[u8BufferSize] = highByte(au16regs[i]);
            u8BufferSize++;
            au8Buffer[u8BufferSize] = lowByte(au16regs[i]);
            u8BufferSize++;
        }
        break;
	case MB_FC_SPEED_CTL:
	case MB_FC_POSITION_CTL:
	case MB_FC_TORQUE_CTL:
	case MB_FC_SETP_CTL:
	case MB_FC_STATUS_FB:
        u8BufferSize = 2;

        for(uint16_t i=0; i< telegram.u16CoilsNo; i++){
			if((u8BufferSize+1)>=MAX_BUFFER){
				u8lastError = MBERR_BUFF_OVERFLOW;
				return;
			}
            au8Buffer[u8BufferSize] = highByte(au16regs[i]);
            u8BufferSize++;
            au8Buffer[u8BufferSize] = lowByte(au16regs[i]);
            u8BufferSize++;
        }
		if(telegram.u8fct!=MB_FC_STATUS_FB){
			au8Buffer[u8BufferSize] = telegram.ack;
			u8BufferSize++;
		}
        break;
	case MB_FC_REPORT_ID:
	case MB_FC_REBOOT:
	case MB_FC_SYNC_ACTION:
		u8BufferSize = 2;
		break;
    }
    // append CRC to message
    uint16_t u16crc = calcCRC(u8BufferSize);
    au8Buffer[u8BufferSize] = u16crc >> 8;
    u8BufferSize++;
    au8Buffer[u8BufferSize] = u16crc & 0x00ff;
    u8BufferSize++;

	if(isFlush){
		flushModbusRx();
		writeModbus(au8Buffer, u8BufferSize);
		flushModbusTx();
	}else{
		writeModbus(au8Buffer, u8BufferSize);		
	}

    u8state = COM_WAITING;
    u8lastError = 0;
    return;
}

uint8_t Rtu::getRxu8Len()
{
	uint8_t rxu8Len;
    switch(telegram.u8fct)
    {
    case MB_FC_READ_COILS:
    case MB_FC_READ_DISCRETE_INPUT:
		rxu8Len = 5+telegram.u16CoilsNo;
        break;
    case MB_FC_READ_INPUT_REGISTER:
    case MB_FC_READ_REGISTERS :
		rxu8Len = 5+telegram.u16CoilsNo*2;
        break;
    case MB_FC_WRITE_COIL:
    case MB_FC_WRITE_REGISTER :
    case MB_FC_WRITE_MULTIPLE_COILS:
    case MB_FC_WRITE_MULTIPLE_REGISTERS:
		if(telegram.u8id == 0){
			rxu8Len = 0;
		}else{
			rxu8Len = 8;
		}
		break;
	case MB_FC_REPORT_ID:
		rxu8Len = 8;
        break;
	case MB_FC_SPEED_CTL:
	case MB_FC_POSITION_CTL:
	case MB_FC_TORQUE_CTL:
	case MB_FC_SETP_CTL:
	case MB_FC_STATUS_FB:
		if(telegram.u8id == 0){
			rxu8Len = 0;
		}else{
			rxu8Len = 16;
		}
		break;
    default:
		rxu8Len = 0;
		u8lastError = MBERR_FUNC_CODE;
        break;
    }
	if(rxu8Len>MAX_BUFFER){
		rxu8Len = 0;
		u8lastError = MBERR_BUFF_OVERFLOW;
	}
    return rxu8Len;
}

/**
 * This method checks if there is any incoming answer if pending.
 * If there is no answer, it would change Master state to COM_IDLE.
 *
 * Any incoming data would be redirected to au16regs pointer,
 * as defined in its modbus_t query telegram.
 *
 * @params	nothing
 */
void Rtu::poll()
{
	uint8_t rxu8Len = getRxu8Len();
	if(rxu8Len == 0){
		u8state = COM_IDLE;
		return;
	}
	u8BufferSize = readModbus(au8Buffer, rxu8Len);
	if(u8BufferSize != rxu8Len){
		u8state = COM_IDLE;
		u8lastError = MBERR_NO_REPLY;
		return;
	}

    // validate message: id, CRC, FCT, exception
    uint8_t u8exception = validateAnswer();
    if(u8exception){
        u8state = COM_IDLE;
		u8lastError = u8exception;
        return;
    }

    // process answer
    switch(au8Buffer[FUNC])
    {
    case MB_FC_READ_COILS:
    case MB_FC_READ_DISCRETE_INPUT:
        // call get_FC1 to transfer the incoming message to au16regs buffer
        get_FC1();
        break;
    case MB_FC_READ_INPUT_REGISTER:
    case MB_FC_READ_REGISTERS :
        // call get_FC3 to transfer the incoming message to au16regs buffer
        get_FC3();
        break;
	case MB_FC_SPEED_CTL:
	case MB_FC_POSITION_CTL:
	case MB_FC_TORQUE_CTL:
	case MB_FC_SETP_CTL:
	case MB_FC_STATUS_FB:
		get_FC4();
		break;
    case MB_FC_WRITE_COIL:
    case MB_FC_WRITE_REGISTER :
    case MB_FC_WRITE_MULTIPLE_COILS:
    case MB_FC_WRITE_MULTIPLE_REGISTERS :
        // nothing to do
        break;
    default:
        break;
    }
    u8state = COM_IDLE;
}

/**
 * This method calculates CRC
 *
 * @return uint16_t calculated CRC value for the message
 * @ingroup buffer
 */
uint16_t Rtu::calcCRC(uint8_t u8length)
{
    unsigned int temp, temp2, flag;
    temp = 0xFFFF;
    for(unsigned char i = 0; i < u8length; i++){
        temp = temp ^ au8Buffer[i];
        for (unsigned char j = 1; j <= 8; j++){
            flag = temp & 0x0001;
            temp >>= 1;
            if (flag)
                temp ^= 0xA001;
        }
    }
    // Reverse byte order.
    temp2 = (temp>>8);
    temp = (temp<<8) | temp2;
    temp &= 0xFFFF;
    // the returned value is already swapped
    // crcLo byte is first & crcHi byte is last
    return temp;
}

/**
 * This method validates master incoming messages
 *
 * @return 0 if OK, EXCEPTION if anything fails
 * @ingroup buffer
 */
uint8_t Rtu::validateAnswer()
{
    // check message crc vs calculated crc
    uint16_t u16MsgCRC = ((au8Buffer[u8BufferSize-2]<<8) | au8Buffer[u8BufferSize-1]);
    if(calcCRC(u8BufferSize-2) != u16MsgCRC){
        return MBERR_CRC_CMP;
    }

	if(au8Buffer[ID] != telegram.u8id && telegram.u8id){
		return MBERR_SLAVE_ID;
	}else{
		slaveID = au8Buffer[ID];
	}
    // check exception
    if((au8Buffer[FUNC] & 0x80) != 0){
        return MBERR_FUNC_CODE;
    }

    // check fct code
    uint8_t isSupported = 0;
    for(uint8_t i = 0; i< sizeof(fctsupported); i++){
        if(fctsupported[i] == au8Buffer[FUNC]){
            isSupported = 1;
            break;
        }
    }
    if(!isSupported){
        return MBERR_FUNC_CODE;
    }

    return 0; // OK, no exception code thrown
}

/**
 * This method processes functions 1 & 2 (for master)
 * This method puts the slave answer into master data buffer
 *
 * @ingroup register
 * TODO: finish its implementation
 */
void Rtu::get_FC1()
{
    uint8_t u8byte, i;
    u8byte = 3;
	uint16_t *au16regs = telegram.au16reg;
     for(i=0; i< au8Buffer[2]; i++){ 
        if(i%2){
            au16regs[i/2]= word(au8Buffer[i+u8byte], lowByte(au16regs[i/2]));
        }else{
            au16regs[i/2]= word(highByte(au16regs[i/2]), au8Buffer[i+u8byte]); 
        }
     }
}

/**
 * This method processes functions 3 & 4 (for master)
 * This method puts the slave answer into master data buffer
 *
 * @ingroup register
 */
void Rtu::get_FC3()
{
    uint8_t u8byte, i;
    u8byte = 3;
	uint16_t *au16regs = telegram.au16reg;
    for(i=0; i<au8Buffer[2]/2; i++){
        au16regs[i] = word(au8Buffer[u8byte], au8Buffer[u8byte+1]);
        u8byte += 2;
    }
}

uint8_t Rtu::writeReg(uint8_t ID, uint16_t regAddr, uint16_t wDat)
{
	telegram.u8id = ID; // slave address
	telegram.u8fct = MB_FC_WRITE_REGISTER; // function code (this one is registers read)
    telegram.u16RegAdd = regAddr; // start address in slave
    telegram.u16CoilsNo = 1; // number of elements (coils or registers) to read
    telegram.au16reg = &wDat; // pointer to a memory array in the Arduino
    query();
    while(u8state != COM_IDLE){
		poll();
    }
    if(!u8lastError){
		return 1;
    }else{
		return 0;
    }
}

uint8_t Rtu::writeReg(uint8_t ID, uint16_t regAddr, uint16_t *wDat, uint8_t nLen)
{
	telegram.u8id = ID; // slave address
	telegram.u8fct = MB_FC_WRITE_MULTIPLE_REGISTERS; // function code (this one is registers read)
    telegram.u16RegAdd = regAddr; // start address in slave
    telegram.u16CoilsNo = nLen; // number of elements (coils or registers) to read
    telegram.au16reg = wDat; // pointer to a memory array in the Arduino
    query();
    while(u8state != COM_IDLE){
		poll();
    }
    if(!u8lastError){
		return 1;
    }else{
		return 0;
    }
}

uint8_t Rtu::readReg(uint8_t ID, uint16_t regAddr, uint16_t *wDat, uint8_t nLen)
{
	telegram.u8id = ID; // slave address
	telegram.u8fct = MB_FC_READ_REGISTERS; // function code (this one is registers read)
    telegram.u16RegAdd = regAddr; // start address in slave
    telegram.u16CoilsNo = nLen; // number of elements (coils or registers) to read
    telegram.au16reg = wDat; // pointer to a memory array in the Arduino
    query();
    while(u8state != COM_IDLE){
		poll();
    }
    if(!u8lastError){
		return nLen;
    }else{
		return 0;
    }
}

int16_t Rtu::readReg(uint8_t ID, uint16_t regAddr)
{
	uint16_t wDat;
	telegram.u8id = ID; // slave address
	telegram.u8fct = MB_FC_READ_REGISTERS; // function code (this one is registers read)
    telegram.u16RegAdd = regAddr; // start address in slave
    telegram.u16CoilsNo = 1; // number of elements (coils or registers) to read
    telegram.au16reg = &wDat; // pointer to a memory array in the Arduino
    query();
    while(u8state != COM_IDLE){
		poll();
    }
    if(!u8lastError){
		return wDat;
    }else{
		return -1;
    }
}

int Rtu::Ping(uint8_t ID)
{
	telegram.u8id = ID; // slave address
	telegram.u8fct = MB_FC_REPORT_ID; // function code (this one is registers read)
    query();
    while(u8state != COM_IDLE){
		poll();
    }
    if(!u8lastError){
		return slaveID;
    }else{
		return -1;
    }
}

void Rtu::reBoot(uint8_t ID)
{
	telegram.u8id = ID; // slave address
	telegram.u8fct = MB_FC_REBOOT; // function code (this one is registers read)
    query();
	u8state = COM_IDLE;
}


void Rtu::writeUser(uint8_t ID, uint16_t regAddr, uint16_t wDat)
{
	telegram.u8id = ID;
	telegram.u8fct = MB_FC_USER_WRITE;
    telegram.u16RegAdd = regAddr;
    telegram.u16CoilsNo = 1;
    telegram.au16reg = &wDat;
    query();
	u8state = COM_IDLE;
}

void Rtu::writeUser(uint8_t ID, uint16_t regAddr, uint16_t *wDat, uint8_t nLen)
{
	telegram.u8id = ID;
	telegram.u8fct = MB_FC_USER_WRITE;
    telegram.u16RegAdd = regAddr;
    telegram.u16CoilsNo = nLen;
    telegram.au16reg = wDat;
    query();
	u8state = COM_IDLE;
}

void Rtu::writeSync(uint8_t ID, uint16_t regAddr, uint16_t wDat)
{
	telegram.u8id = ID;
	telegram.u8fct = MB_FC_SYNC_WRITE;
    telegram.u16RegAdd = regAddr;
    telegram.u16CoilsNo = 1;
    telegram.au16reg = &wDat;
    query();
	u8state = COM_IDLE;
}

void Rtu::writeSync(uint8_t ID, uint16_t regAddr, uint16_t *wDat, uint8_t nLen)
{
	telegram.u8id = ID;
	telegram.u8fct = MB_FC_SYNC_WRITE;
    telegram.u16RegAdd = regAddr;
    telegram.u16CoilsNo = nLen;
    telegram.au16reg = wDat;
    query();
	u8state = COM_IDLE;
}

void Rtu::actionSync(uint8_t ID)
{
	telegram.u8id = ID;
	telegram.u8fct = MB_FC_SYNC_ACTION;
    query();
	u8state = COM_IDLE;
}

void Rtu::get_FC4()
{
    uint8_t u8byte, i;
    u8byte = 2;
	uint16_t *au16regs = telegram.au16reg;
    for(i=0; i<6; i++){
        au16regs[i] = word(au8Buffer[u8byte], au8Buffer[u8byte+1]);
        u8byte += 2;
    }
}
