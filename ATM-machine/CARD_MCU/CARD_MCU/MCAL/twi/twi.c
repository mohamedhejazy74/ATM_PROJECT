/*
 * twi.c
 *
 * Created: 5/6/2023 5:19:07 PM
 *  Author: Mahmoud Sarhan
 */ 


#include "twi.h"
#define INIT	0x01
#define N_INIT	0x00
static uint8_t u8_gs_twi_state = N_INIT;


// function to get the status of TWI
uint8_t TWI_get_status(void)
{
	uint8_t status ;
	status = TWSR & 0xf8;
	return status;
}


u8_twiErrorType TWI_init(st_twiConfigType * st_twiConfig)
{
	u8_twiErrorType  u8_ret_val = TWI_ERROR_OK;
	if (st_twiConfig == NULL)
	{
		u8_ret_val = TWI_ERROR_NOT_OK;
	}
	else{
		if ((st_twiConfig->u16_a_clock) == 0 || (st_twiConfig->u16_a_clock) > 400)
		{
			u8_ret_val = TWI_ERROR_NOT_OK;
		}
		else{
			// Initialize TWI driver
			TWSR = 0x00;	// Clear status register
			TWSR |= st_twiConfig->u8_a_prescaler;		// Set prescaler 
			TWBR = BIT_RATE(st_twiConfig->u16_a_clock , st_twiConfig->u8_a_prescaler);	// Set bit rate
			u8_gs_twi_state = INIT;
		}
	}
	return u8_ret_val;
}

u8_twiErrorType TWI_start(void)
{
	u8_twiErrorType  u8_ret_val = TWI_ERROR_OK;
	if (u8_gs_twi_state == INIT)
	{
		// SET TWI to start mode
		TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		// WAIT till TWI start
		while ((TWCR & (1<<TWINT))==0);
		if (TWI_get_status() == START_STATE)
		{
			// do nothing
		}
		else{
			u8_ret_val = TWI_ERROR_NOT_OK;
		}
	}
	else{
		u8_ret_val = TWI_ERROR_NOT_OK;
	}
	return u8_ret_val;
}


u8_twiErrorType TWI_repeated_start(void)
{
	u8_twiErrorType  u8_ret_val = TWI_ERROR_OK;
	if (u8_gs_twi_state == INIT)
	{
		// SET TWI to start mode
		TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		// WAIT till TWI start
		while ((TWCR & (1<<TWINT))==0);
		if (TWI_get_status() == REPEATED_START_STATE)
		{
			// do nothing
		}
		else{
			u8_ret_val = TWI_ERROR_NOT_OK;
		}
	}
	else{
		u8_ret_val = TWI_ERROR_NOT_OK;
	}
	return u8_ret_val;
}


u8_twiErrorType TWI_stop(void)
{
	u8_twiErrorType  u8_ret_val = TWI_ERROR_OK;
	if (u8_gs_twi_state == INIT)
	{
		// SET TWI to stop mode
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	}
	else{
		u8_ret_val = TWI_ERROR_NOT_OK;
	}
	return u8_ret_val;
}



u8_twiErrorType TWI_setAddress(uint8_t u8_address,uint8_t u8_rw)
{
	u8_twiErrorType u8_ret_val = TWI_ERROR_OK;
	if (u8_gs_twi_state == INIT)
	{
		if (u8_rw == READ || u8_rw == WRITE)
		{
			TWDR = u8_address + u8_rw;
			TWCR = (1<<TWINT)|(1<<TWEN);
			while ((TWCR & (1<<TWINT))==0);
			if (TWI_get_status() == SLA_W_ACK_STATE || TWI_get_status() == SLA_W_NACK_STATE)
			{
				// do nothing
			}
			else{
				u8_ret_val = TWI_ERROR_NOT_OK;
				//u8_ret_val = 1;
			}
		}
		else
		{
			u8_ret_val = TWI_ERROR_NOT_OK;
			//u8_ret_val = 2;
		}
	}
	else{
		u8_ret_val = TWI_ERROR_NOT_OK;
		//u8_ret_val = 3;
	}
	return u8_ret_val;
}

u8_twiErrorType TWI_wrtie(uint8_t u8_data)
{
	u8_twiErrorType u8_ret_val = TWI_ERROR_OK;
	if (u8_gs_twi_state == INIT)
	{
		TWDR = u8_data;
		TWCR = (1<<TWINT)|(1<<TWEN);
		while ((TWCR & (1<<TWINT))==0);
		if (TWI_get_status() == DATA_W_ACK_STATE || TWI_get_status() == DATA_W_NACK_STATE)
		{
			// do nothing
		}
		else{
			u8_ret_val = TWI_ERROR_NOT_OK;
		}
	}
	else{
		u8_ret_val = TWI_ERROR_NOT_OK;
	}
	return u8_ret_val;
}
u8_twiErrorType TWI_read(uint8_t* u8_data , uint8_t u8_ack)
{
	u8_twiErrorType u8_ret_val = TWI_ERROR_OK;
	if (u8_gs_twi_state == INIT)
	{
		if (u8_ack == ACK)
		{
			TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
		}
		else if (u8_ack == NACK)
		{
			TWCR = (1<<TWINT)|(1<<TWEN);
		}
		else{
			// do nothing
		}
		while ((TWCR & (1<<TWINT))==0);
		* u8_data = TWDR;
		if (TWI_get_status() == DATA_R_ACK_STATE || TWI_get_status() == DATA_R_NACK_STATE)
		{
			// do nothing
		}
		else{
			u8_ret_val = TWI_ERROR_NOT_OK;
		}
	}
	else{
		u8_ret_val = TWI_ERROR_NOT_OK;
	}
	return u8_ret_val;
}