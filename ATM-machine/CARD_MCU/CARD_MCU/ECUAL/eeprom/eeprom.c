/*
 * eeprom.c
 *
 * Created: 5/9/2023 9:10:21 AM
 *  Author: Mcs
 */ 
#include "eeprom.h"

static st_twiConfigType st_sg_twiConfigObj;



u8_eepromErorrState_t EEPROM_init(u8_twiPrescalerType u8_a_twiPrescaler, uint16_t u8_a_twiClock)
{
	u8_eepromErorrState_t u8_l_ret = EEPROM_E_OK;
	if((u8_a_twiPrescaler <TWI_PRESCALER_1) || (u8_a_twiPrescaler > TWI_PRESCALER_64))
	{
		u8_l_ret = EEPROM_E_NOK;
	}
	else
	{
		st_sg_twiConfigObj.u16_a_clock = u8_a_twiClock;
		st_sg_twiConfigObj.u8_a_prescaler = u8_a_twiPrescaler;
		u8_l_ret = TWI_init(&st_sg_twiConfigObj);
	}
	return u8_l_ret;
}

u8_eepromErorrState_t EEPROM_writeByte(uint16_t u16_a_address,uint8_t u8_a_data)
{
	u8_eepromErorrState_t u8_l_ret = EEPROM_E_OK;
	/* Send the Start Bit */
	u8_l_ret = TWI_start();
	/* Send the device address, we need to get A8 A9 A10 address bits from the memory location address and R/W=0 (write) [device_add + page_add + control_bit]*/
	u8_l_ret |= TWI_setAddress((uint8_t)GET_DEVICE_AND_PAGE_ADDRESS(u16_a_address),WRITE);
	 /*Send the word address (low byte)*/
	u8_l_ret |= TWI_wrtie((uint8_t)u16_a_address);
	/* write data byte to EEPROM */
	u8_l_ret |= TWI_wrtie(u8_a_data);
	/* Send the Stop Bit */
	u8_l_ret |= TWI_stop();
	return u8_l_ret;

}
u8_eepromErorrState_t EEPROM_readByte(uint16_t u16_a_address,uint8_t* u8Ptr_a_data)
{
	u8_eepromErorrState_t u8_l_ret = EEPROM_E_OK;
	if(NULL==u8Ptr_a_data)
	{
		 u8_l_ret = EEPROM_E_NOK;
	}
	else
	{
		/* Send the Start Bit */
		u8_l_ret = TWI_start();
		/* Send the device address, we need to get A8 A9 A10 address bits from the memory location address and R/W=0 (write) [device_add + page_add + control_bit]*/
		u8_l_ret |= TWI_setAddress((uint8_t)GET_DEVICE_AND_PAGE_ADDRESS(u16_a_address),WRITE);
		/*Send the word address (low byte)*/
		u8_l_ret |= TWI_wrtie((uint8_t)u16_a_address);
		/* Send the Repeated Start Bit */
		u8_l_ret |= TWI_repeated_start();
		/* Send the device address, we need to get A8 A9 A10 address bits from the memory location address and R/W=0 (write) [device_add + page_add + control_bit]*/
		u8_l_ret |= TWI_setAddress((uint8_t)GET_DEVICE_AND_PAGE_ADDRESS(u16_a_address),READ);
		/* Read Byte from Memory without send ACK */
		u8_l_ret |= TWI_read(u8Ptr_a_data,NACK);
		/* Send the Stop Bit */
		u8_l_ret |= TWI_stop();
	}
	return u8_l_ret;

}