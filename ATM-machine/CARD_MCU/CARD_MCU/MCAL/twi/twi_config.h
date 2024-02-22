/*
 * twi_config.h
 *
 * Created: 5/6/2023 9:58:25 PM
 *  Author: Mahmoud Sarhan
 */ 


#ifndef TWI_CONFIG_H_
#define TWI_CONFIG_H_


#ifndef	 F_CPU
# define F_CPU 8000000UL
#endif

#include "../../STD_LIBRARIES/STD_TYPES.h"
#include "../../STD_LIBRARIES/bit_math.h"
#include <avr/io.h>
#include <math.h>
typedef uint8_t u8_twiErrorType;

#define TWI_ERROR_OK		((u8_twiErrorType)0x00)
#define TWI_ERROR_NOT_OK	((u8_twiErrorType)0x01)

typedef uint8_t u8_twiPrescalerType;

#define TWI_PRESCALER_1			((u8_twiPrescalerType)0x00)
#define TWI_PRESCALER_4			((u8_twiPrescalerType)0x01)
#define TWI_PRESCALER_16		((u8_twiPrescalerType)0x02)
#define TWI_PRESCALER_64		((u8_twiPrescalerType)0x03)

typedef struct{
	u8_twiPrescalerType u8_a_prescaler;
	uint16_t u16_a_clock;
} st_twiConfigType;


#define BIT_RATE(SCL,Prescaler)			((F_CPU/SCL)-16)/(2*pow(4,Prescaler))



#define ACK				0x00
#define NACK			0x01


#define  READ			0x01
#define  WRITE			0x00

#define START_STATE				0x08
#define REPEATED_START_STATE	0x10
#define SLA_W_ACK_STATE			0x18
#define SLA_W_NACK_STATE		0x20
#define SLA_R_ACK_STATE			0x40
#define SLA_R_NACK_STATE		0x48
#define DATA_W_ACK_STATE		0x28
#define DATA_W_NACK_STATE		0x30
#define DATA_R_ACK_STATE		0x50
#define DATA_R_NACK_STATE		0x58

#endif /* TWI_CONFIG_H_ */