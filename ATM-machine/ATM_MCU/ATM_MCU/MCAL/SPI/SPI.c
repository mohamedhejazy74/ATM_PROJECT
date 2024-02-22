/*
 * SPI.c
 *
 * Created: 5/3/2023 3:42:36 PM
 *  Author: Arafa
 */ 
#include "SPI.h"

SPIErrorsType spi_init(spi_config_t *spi_config){
	SPIErrorsType ret = E_OK;
	if(spi_config == NULL){
		ret =E_NOT_OK;
	}
	else{
		if(spi_config->spi_mode_config == SPI_MASTER){
			
			DIO_init (portb, SPI_SS, STD_OUTPUT);
			DIO_writePIN (portb, SPI_SS, STD_HIGH);	
			DIO_init (portb, SPI_MOSI, STD_OUTPUT);
			DIO_init (portb, SPI_SCK, STD_OUTPUT);
			DIO_init (portb, SPI_MISO, STD_INPUT);
			
			
			}else if(spi_config->spi_mode_config == SPI_SLAVE){
				
			DIO_init (portb, SPI_SS, STD_INPUT);
			DIO_init (portb, SPI_MOSI, STD_INPUT);
			DIO_init (portb, SPI_SCK, STD_INPUT);
			DIO_init (portb, SPI_MISO, STD_OUTPUT);

			

		}
		SPCR_ADD = (spi_config->spi_data_order_config<< SPI_DORD)|(spi_config->spi_mode_config<< SPI_MSTR)
		|(spi_config->spi_interrupt_config<< SPI_SPIE)|(spi_config->spi_chpa_config<< SPI_CPHA)
		|(spi_config->spi_polarity_config<< SPI_CPOL)|(spi_config->spi_prescaller_config<< SPI_SPR);
		
		SPSR_ADD = (SPSR_ADD&0xF0)|(spi_config->spi_prescaller_config>>3);
		SET_BIT(SPCR_ADD, SPI_SPE);
		

	}
	return ret;
}
SPIErrorsType spi_slave_CE_init(spi_slave_config_t *spi_slave_config){
	SPIErrorsType ret = E_OK;
	if(spi_slave_config == NULL){
		ret =E_NOT_OK;
	}
	else{
		
		DIO_init (spi_slave_config->spi_slave_port, spi_slave_config->spi_slave_pin, STD_OUTPUT);
		DIO_writePIN (spi_slave_config->spi_slave_port, spi_slave_config->spi_slave_pin, STD_HIGH);
		
	}
	return ret;
}

SPIErrorsType spi_write(uint8_t *data){
	SPIErrorsType ret = E_OK;
	if(data == NULL){
		ret =E_NOT_OK;
	}
	else{
		uint8_t flush=0;
		SPDR_ADD = *data;
		while(READ_BIT(SPSR_ADD,SPI_SPIF) == 0);
		flush = SPDR_ADD;
		

	}
	return ret;
}
SPIErrorsType spi_read(uint8_t *data){
	SPIErrorsType ret = E_OK;
	if(data == NULL){
		ret =E_NOT_OK;
	}
	else{

		uint8_t flush= 0;
		SPDR_ADD = flush;
		while(READ_BIT(SPSR_ADD,SPI_SPIF) == 0);
		*data = SPDR_ADD;
	}
	return ret;
	
}
SPIErrorsType spi_write_read(uint8_t *transmited_data, uint8_t *recieved_data){
	SPIErrorsType ret = E_OK;
	if( transmited_data == NULL ||recieved_data == NULL ){
		ret =E_NOT_OK;
	}
	else{
		SPDR_ADD = *transmited_data;
		while(READ_BIT(SPSR_ADD,SPI_SPIF) == 0);
		*recieved_data = SPDR_ADD;

	}
	return ret;
}


SPIErrorsType spi_disable_slave(spi_slave_config_t *spi_slave_config){
	SPIErrorsType ret = E_OK;
	if( spi_slave_config == NULL){
		ret =E_NOT_OK;
	}
	else{
		DIO_writePIN (spi_slave_config->spi_slave_port, spi_slave_config->spi_slave_pin, STD_HIGH);
	}
	return ret;
}
SPIErrorsType spi_enable_slave(spi_slave_config_t *spi_slave_config){
	SPIErrorsType ret = E_OK;
	if( spi_slave_config == NULL){
		ret =E_NOT_OK;
	}
	else{
		DIO_writePIN (spi_slave_config->spi_slave_port, spi_slave_config->spi_slave_pin, STD_LOW);
	}
	return ret;
}