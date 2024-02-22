/*
 * CARD_MCU.c
 *
 * Created: 5/3/2023 2:44:07 PM
 * Author : engma
 */ 

//#include <avr/io.h>
//#include <util/delay.h>

#include "MCAL/usart/usart.h"

uint8_t data[6] ;
int main(void)
{
	u8_usartErorrState_t l_ret = USART_E_OK;

	st_usart_config_t st_l_usartObj = {
		.usartBaudRate=9600,
		.usartDataSize = USART_EIGHT_BIT_DATA,
		.usartMode = USART_ASYNCHRONOUS_NORMAL_SPEED_MODE,
		.usartParityBit = USART_DISABLED_PARITY_BIT,
		.usartRxEnable = USART_RX_ENABLE,
		.usartTxEnable = USART_TX_ENABLE,
		.usartRxInterrupt = USART_RX_INTERRUPT_DISABLE,
		.usartTxInterrupt = USART_TX_INTERRUPT_DISABLE,
		.usartStopBitNum = USART_ONE_STOP_BIT,
		};
		l_ret = USART_init(&st_l_usartObj);
		//UCSRC = (1<<URSEL) | (3<<UCSZ0);
    /* Replace with your application code */
    while (1) 
    {
		
		l_ret |= USART_reciveString(&st_l_usartObj,data);
		
		l_ret |= USART_sendString(&st_l_usartObj,data);
    }
}

