/*
 * twi.h
 *
 * Created: 5/6/2023 5:19:26 PM
 *  Author: Mahmoud Sarhan
 */ 


#ifndef TWI_H_
#define TWI_H_

#include "twi_config.h"

u8_twiErrorType TWI_init(st_twiConfigType * st_twiConfig);
u8_twiErrorType TWI_start(void);
u8_twiErrorType TWI_repeated_start(void);
u8_twiErrorType TWI_stop(void);
u8_twiErrorType TWI_setAddress(uint8_t u8_address,uint8_t u8_rw);
u8_twiErrorType TWI_wrtie(uint8_t u8_data);
u8_twiErrorType TWI_read(uint8_t* u8_data , uint8_t u8_ack);






#endif /* TWI_H_ */