/*
 * ATM_MCU.c
 *
 * Created: 5/3/2023 2:48:21 PM
 * Author : engma
 */ 
#define F_CPU 1000000ul
//#include <avr/io.h>
#include <util/delay.h>
#include "MCAL/SPI/SPI.h"
spi_config_t spi_config;
spi_slave_config_t spi_slave_config;
uint8_t data =0;
int main(void)
{
	spi_config.spi_interrupt_config = SPI_INT_DISABLE;
	spi_config.spi_state_config = SPI_ENABLE;
	spi_config.spi_data_order_config = SPI_MSB;
	spi_config.spi_mode_config = SPI_MASTER;
	spi_config.spi_polarity_config = SPI_IDLE_LOW;
	spi_config.spi_chpa_config = SPI_SAMLING_ON_LEADING_EDGE;
	spi_config.spi_prescaller_config = SPI_PRESCALER_4;
	spi_init(&spi_config);
	
	spi_slave_config.spi_slave_port = portb;
	spi_slave_config.spi_slave_pin = pin4;
	spi_slave_CE_init(&spi_slave_config);
    /* Replace with your application code */
    while (1) 
    {
		spi_enable_slave(&spi_slave_config);
		spi_read(&data);
		data++;
		_delay_ms(500);
		spi_read(&data);
		data++;
		spi_read(&data);
		data++;
		spi_read(&data);
		data++;
		spi_disable_slave(&spi_slave_config);
    }
}

