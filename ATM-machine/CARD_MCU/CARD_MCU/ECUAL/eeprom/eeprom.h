/*
 * eeprom.h
 *
 * Created: 5/9/2023 9:10:08 AM
 *  Author: Mcs
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_


/* ------------------------------------------------------- Includes ---------------------------------------------------------------------*/
#include "../../STD_LIBRARIES/STD_TYPES.h"
#include "../../STD_LIBRARIES/bit_math.h"
#include "../../MCAL/twi/twi.h"

/* ------------------------------------------------------Data Type Declarations ----------------------------------------------------------*/
typedef uint8_t u8_eepromErorrState_t;


/* ------------------------------------------------------ Macro Declarations ------------------------------------------------------------*/



#define  EEPROM_CONTROL_BYTE_WRITE_OP		()
/*The Error state of The EEPROM*/
#define EEPROM_E_OK		 ((u8_eepromErorrState_t)0x00)
#define EEPROM_E_NOK     ((u8_eepromErorrState_t)0x01)
/* ------------------------------------------------------ Macro Like Functions Declarations ---------------------------------------------*/

/* Send the device address, we need to get A8 A9 A10 address bits from the memory location address and R/W=0 (write) [device_add + page_add + control_bit]*/
#define  GET_DEVICE_AND_PAGE_ADDRESS(ADDRESS)		(0xA0 | (((uint16_t)ADDRESS & 0x0700)>>7))





/* ------------------------------------------------- Software Interfaces Declarations ---------------------------------------------------*/

u8_eepromErorrState_t EEPROM_init(u8_twiPrescalerType u8_a_twiPrescaler, uint16_t u8_a_twiClock);

u8_eepromErorrState_t EEPROM_writeByte(uint16_t u16_a_address,uint8_t u8_a_data);

u8_eepromErorrState_t EEPROM_readByte(uint16_t u16_a_address,uint8_t* u8Ptr_a_data);





#endif /* EEPROM_H_ */


