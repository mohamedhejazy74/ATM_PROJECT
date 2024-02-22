/*
 * usart.c
 *
 * Created: 5/1/2023 7:35:51 AM
 *  Author: Mohamed Abdel-Wahab
 *	EMAIL : mohamedhegay22@gmail.com
 */ 
#include "usart.h"



static volatile uint8_t u8_sg_dataTransimted;
static volatile uint8_t u8_sg_dataRecieved;

ISR(USART_UDRE_vect)
{

	UDR= u8_sg_dataTransimted;
}

ISR(USART_RXC_vect)
{

	u8_sg_dataRecieved= UDR;
}


u8_usartErorrState_t USART_init(const st_usart_config_t *stPtr_a_usartConfig)
{
	u8_usartErorrState_t l_ret = USART_E_OK;
	float32_t f32_l_baudRatePrescaler;
	if(NULL==stPtr_a_usartConfig || stPtr_a_usartConfig->usartMode >= USART_INVALID_MODE || stPtr_a_usartConfig->usartDataSize >= USART_INVALID_BIT_DATA
	|| stPtr_a_usartConfig->usartParityBit >= USART_INVALID_PARITY_BIT || stPtr_a_usartConfig->usartStopBitNum >= USART_INVALID_STOP_BITS)
	{
		l_ret = USART_E_NOK;
	}
	else
	{
		/*calculate the value on UBRR -high & low- with the defined baudRate */
		
		switch(stPtr_a_usartConfig->usartMode)
		{
			case USART_ASYNCHRONOUS_NORMAL_SPEED_MODE :
				f32_l_baudRatePrescaler =  ( F_CPU / ((stPtr_a_usartConfig->usartBaudRate) * 16.0))  - 1;
				 UBRRH = (uint8_t)(((uint32_t)f32_l_baudRatePrescaler) >> 8);
				 UBRRL = (uint8_t)((uint32_t)f32_l_baudRatePrescaler);	
				break;
			case USART_ASYNCHRONOUS_DOUBLE_SPEED_MODE :
				f32_l_baudRatePrescaler =  ( F_CPU / ((stPtr_a_usartConfig->usartBaudRate) * 8.0) ) - 1;
				UBRRH = (uint8_t)(((uint32_t)f32_l_baudRatePrescaler) >> 8);
				UBRRL = (uint8_t)((uint32_t)f32_l_baudRatePrescaler);	
				break;		
			default:
				l_ret = USART_E_NOK;
				break;
		}
		/*The URSEL-BIT must be one when writing on UCSRC register*/
		SET_BIT(UCSRC,URSEL);
		/*set USART mode*/
		switch(stPtr_a_usartConfig->usartMode)
		{
			case USART_ASYNCHRONOUS_NORMAL_SPEED_MODE :
				//CLEAR UMSEL BIT TO SELECT ASYNCHRONOUS MODE
				CLEAR_BIT(UCSRC,UMSEL);
				break;
			case USART_ASYNCHRONOUS_DOUBLE_SPEED_MODE :
				//CLEAR UMSEL BIT TO SELECT ASYNCHRONOUS MODE
				CLEAR_BIT(UCSRC,UMSEL);
				//SET THE U2X TO DOUBLE THE USART TRANSMISSION SPEED
				SET_BIT(UCSRA,U2X);
				break;
			case USART_SYNCHRONOUS_MODE :
				//SET UMSEL BIT TO SELECT SYNCHRONOUS MODE
				SET_BIT(UCSRC,UMSEL);
				//SELCET THE CLOCK POLARITY IN CASE OF SYNCHRONOUS MODE ONLY 
				if((stPtr_a_usartConfig->usartTxClkPolarity==USART_SYNCHRONOUS_TX_RISING_XCK_EDGE)||
				   (stPtr_a_usartConfig->usartRxClkPolarity==USART_SYNCHRONOUS_RX_FALLING_XCK_EDGE))
				{
					CLEAR_BIT(UCSRC,UCPOL);
				}
				else if((stPtr_a_usartConfig->usartTxClkPolarity==USART_SYNCHRONOUS_TX_FALLING_XCK_EDGE)||
						(stPtr_a_usartConfig->usartRxClkPolarity==USART_SYNCHRONOUS_RX_RISING_XCK_EDGE))
				{
					SET_BIT(UCSRC,UCPOL);
				}
				else
				{
					l_ret = USART_E_NOK;
				}
				
				break;
			default:
				l_ret = USART_E_NOK;
				break;		
		}
		/*enable transmitter if set*/
		if(stPtr_a_usartConfig->usartTxEnable==USART_TX_ENABLE)
		{
			SET_BIT(UCSRB,TXEN);
		}
		else
		{
			//do nothing
		}
		
		/*enable receiver if set*/
		if(stPtr_a_usartConfig->usartRxEnable==USART_RX_ENABLE)
		{
			SET_BIT(UCSRB,RXEN);
		}
		else
		{
			//do nothing
		}
		
		/*enable transmitter interrupt if set*/
		if(stPtr_a_usartConfig->usartTxInterrupt==USART_TX_INTERRUPT_ENABLE)
		{
			//TX Complete Interrupt Enable
			SET_BIT(UCSRB,TXCIE);
			//USART Data Register Empty Interrupt Enable
			SET_BIT(UCSRB,UDRIE);
		}
		else
		{
			//do nothing
		}
		/*enable receiver if interrupt set*/
		if(stPtr_a_usartConfig->usartRxInterrupt==USART_RX_INTERRUPT_ENABLE)
		{
			//RX Complete Interrupt Enable
			SET_BIT(UCSRB,RXCIE);
		}
		else
		{
			//do nothing
		}
		
		/*adjust frame format*/
		
		//Select Number of stop-bit either one or two
		switch(stPtr_a_usartConfig->usartStopBitNum)
		{
			case USART_ONE_STOP_BIT:
				CLEAR_BIT(UCSRC,USBS);
				break;
			case USART_TWO_STOP_BITS:
				SET_BIT(UCSRC,USBS);
				break;
			default:
				l_ret = USART_E_NOK;
				break;
		}
		
		//Select Parity mode or disabled parity
		switch(stPtr_a_usartConfig->usartParityBit)
		{
			case USART_DISABLED_PARITY_BIT:
				CLEAR_BIT(UCSRC,UPM0);
				CLEAR_BIT(UCSRC,UPM1);
				break;
			case USART_EVEN_PARITY_BIT:
				CLEAR_BIT(UCSRC,UPM0);
				SET_BIT(UCSRC,UPM1);
				break;
			case USART_ODD_PARITY_BIT:
				SET_BIT(UCSRC,UPM0);
				SET_BIT(UCSRC,UPM1);
				break;
			default:
				l_ret = USART_E_NOK;
				break;
		}
		//Select the data-bit number
		switch(stPtr_a_usartConfig->usartDataSize)
		{
			case USART_FIVE_BIT_DATA:
				CLEAR_BIT(UCSRC,UCSZ0);
				CLEAR_BIT(UCSRC,UCSZ1);
				CLEAR_BIT(UCSRB,UCSZ2);
				break;
			case USART_SIX_BIT_DATA:
				SET_BIT(UCSRC,UCSZ0);
				CLEAR_BIT(UCSRC,UCSZ1);
				CLEAR_BIT(UCSRB,UCSZ2);
				break;
			case USART_SEVEN_BIT_DATA:
				CLEAR_BIT(UCSRC,UCSZ0);
				SET_BIT(UCSRC,UCSZ1);
				CLEAR_BIT(UCSRB,UCSZ2);
				break;
			case USART_EIGHT_BIT_DATA:
				SET_BIT(UCSRC,UCSZ0);
				SET_BIT(UCSRC,UCSZ1);
				CLEAR_BIT(UCSRB,UCSZ2);
				break;
			case USART_NINE_BIT_DATA:
				SET_BIT(UCSRC,UCSZ0);
				SET_BIT(UCSRC,UCSZ1);
				SET_BIT(UCSRB,UCSZ2);
				break;
			default:
				l_ret = USART_E_NOK;
				break;
		}	
	}
	return l_ret;
}

u8_usartErorrState_t USART_sendData(const st_usart_config_t *stPtr_a_usartConfig ,  uint8_t u8_a_data)
{
	u8_usartErorrState_t l_ret = USART_E_OK;
	
	if(NULL==stPtr_a_usartConfig || stPtr_a_usartConfig->usartMode >= USART_INVALID_MODE || stPtr_a_usartConfig->usartDataSize >= USART_INVALID_BIT_DATA
	|| stPtr_a_usartConfig->usartParityBit >= USART_INVALID_PARITY_BIT || stPtr_a_usartConfig->usartStopBitNum >= USART_INVALID_STOP_BITS)
	{
		l_ret = USART_E_NOK;
	}
	else
	{
		if(stPtr_a_usartConfig->usartTxInterrupt==USART_TX_INTERRUPT_DISABLE)
		{
			/*if not using interrupt then will use blocking technique*/
			// UDRE flag is set when the Tx buffer (UDR) is empty and ready for transmitting a new byte so wait until this flag is set to one
			while(!READ_BIT(UCSRA,UDRE));
			UDR = u8_a_data;
		}
		else if(stPtr_a_usartConfig->usartTxInterrupt==USART_TX_INTERRUPT_ENABLE)
		{
			/*when Tx buffer (UDR) is empty and ready for transmitting a new byte so wait until this flag is set to one and will generate an interrupt*/
			u8_sg_dataTransimted = u8_a_data ;
		}
		else
		{
			l_ret = USART_E_NOK;
		}
	}
	return l_ret;
}


u8_usartErorrState_t USART_reciveData(const st_usart_config_t *stPtr_a_usartConfig , uint8_t *const u8Ptr_a_data)
{
	u8_usartErorrState_t l_ret = USART_E_OK;
	
	if(NULL==stPtr_a_usartConfig||NULL==u8Ptr_a_data || stPtr_a_usartConfig->usartMode >= USART_INVALID_MODE || stPtr_a_usartConfig->usartDataSize >= USART_INVALID_BIT_DATA
	|| stPtr_a_usartConfig->usartParityBit >= USART_INVALID_PARITY_BIT || stPtr_a_usartConfig->usartStopBitNum >= USART_INVALID_STOP_BITS)
	{
		l_ret = USART_E_NOK;
	}
	else
	{
		if(stPtr_a_usartConfig->usartRxInterrupt==USART_RX_INTERRUPT_DISABLE)
		{
			/*if not using interrupt then will use blocking technique*/
			//RXC flag is set when the UART receive data so wait until this flag is set to one
			while(!READ_BIT(UCSRA,RXC));
			*u8Ptr_a_data = UDR;
		}
		else if(stPtr_a_usartConfig->usartRxInterrupt==USART_RX_INTERRUPT_ENABLE)
		{
			/*RXC flag is set when the UART receive data so wait until this flag is set to one and will generate an interrupt*/
			*u8Ptr_a_data = u8_sg_dataRecieved;
		}
		else
		{
			l_ret = USART_E_NOK;
		}
	}
	return l_ret;	
}

u8_usartErorrState_t USART_sendString(const st_usart_config_t *stPtr_a_usartConfig , uint8_t *u8Arr_a_stringOfData)
{
	u8_usartErorrState_t l_ret = USART_E_OK;
	uint8_t u8_l_index=U8_ZERO_INITIALIZE;
	
	if(NULL==stPtr_a_usartConfig||NULL==u8Arr_a_stringOfData || stPtr_a_usartConfig->usartMode >= USART_INVALID_MODE || stPtr_a_usartConfig->usartDataSize >= USART_INVALID_BIT_DATA
	|| stPtr_a_usartConfig->usartParityBit >= USART_INVALID_PARITY_BIT || stPtr_a_usartConfig->usartStopBitNum >= USART_INVALID_STOP_BITS)
	{
		l_ret = USART_E_NOK;
	}
	else
	{
		while(u8Arr_a_stringOfData[u8_l_index] != '\0')
		{
			l_ret = USART_sendData(stPtr_a_usartConfig,u8Arr_a_stringOfData[u8_l_index]);
			u8_l_index++;
		}
	}
	return l_ret;
}

u8_usartErorrState_t USART_reciveString(const st_usart_config_t *stPtr_a_usartConfig ,uint8_t *const u8Arr_a_stringOfData)
{
		u8_usartErorrState_t l_ret = USART_E_OK;
		uint8_t u8_l_index=U8_ZERO_INITIALIZE;
		uint8_t u8_l_breakloopFalg=U8_ZERO_INITIALIZE;
		
		if(NULL==stPtr_a_usartConfig||NULL==u8Arr_a_stringOfData || stPtr_a_usartConfig->usartMode >= USART_INVALID_MODE || stPtr_a_usartConfig->usartDataSize >= USART_INVALID_BIT_DATA
		|| stPtr_a_usartConfig->usartParityBit >= USART_INVALID_PARITY_BIT || stPtr_a_usartConfig->usartStopBitNum >= USART_INVALID_STOP_BITS)
		{
			l_ret = USART_E_NOK;
		}
		else
		{
			while(!u8_l_breakloopFalg)
			{
				 l_ret = USART_reciveData(stPtr_a_usartConfig,&u8Arr_a_stringOfData[u8_l_index]);
				 if(u8Arr_a_stringOfData[u8_l_index] == END_OF_STRING_SYMPOL)
				 {
					 u8_l_breakloopFalg++;
					 u8Arr_a_stringOfData[u8_l_index] = '\0';	 
				 }
				 else
				 {
					 //do nothing
				 }
				 u8_l_index++;
			}
		}
		return l_ret;
}