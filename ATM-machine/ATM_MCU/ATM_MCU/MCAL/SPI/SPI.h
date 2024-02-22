/*
 * SPI.h
 *
 * Created: 4/23/2023 7:38:22 PM
 *  Author: Arafa
 */ 


#ifndef SPI_H_
#define SPI_H_
#include "../dio/dio.h"
#include "../../STD_LIBRARIES/bit_math.h"
#include "../../STD_LIBRARIES/STD_TYPES.h"

#define SPI_PORT	portb
#define SPI_SS		pin4
#define SPI_MOSI	pin5
#define SPI_MISO	pin6
#define SPI_SCK		pin7


#define SPCR_ADD	(*((volatile uint8_t *) 0x2D)) 
#define SPI_SPIE	7
#define SPI_SPE		6
#define SPI_DORD	5
#define SPI_MSTR	4
#define SPI_CPOL	3
#define SPI_CPHA	2
#define SPI_SPR		0

#define SPSR_ADD	(*((volatile uint8_t *) 0x2E)) 
#define SPI_SPIF	7
#define SPI_WCOL	6
#define SPI_SPR2X	0

#define SPDR_ADD	(*((volatile uint8_t *) 0x2F))

typedef enum {
    SPI_E_OK,
    SPI_E_NOK
}SPIErrorsType;

typedef enum{
	SPI_INT_DISABLE=0,
	SPI_INT_ENABLE
}spi_interrupt_config_t;
typedef enum{
	SPI_DISABLE=0,
	SPI_ENABLE
}spi_state_config_t;
typedef enum{
	SPI_MSB=0,
	SPI_LSB
}spi_data_order_config_t;

typedef enum{
	SPI_SLAVE=0,
	SPI_MASTER
}spi_mode_config_t;
typedef enum{
	SPI_IDLE_LOW=0,
	SPI_IDLE_HIGH
}spi_polarity_config_t;
typedef enum{
	SPI_SAMLING_ON_LEADING_EDGE=0,
	SPI_SAMLING_ON_TRAILING_EDGE
}spi_chpa_config_t;
typedef enum{
	SPI_SLAVE_NO_PRESCALER=0,
	SPI_PRESCALER_4=0,
	SPI_PRESCALER_16,
	SPI_PRESCALER_64,
	SPI_PRESCALER_128,
	SPI_PRESCALER_2,
	SPI_PRESCALER_8,
	SPI_PRESCALER_32,
}spi_prescaller_config_t;

typedef struct{
	spi_interrupt_config_t spi_interrupt_config;
	spi_state_config_t spi_state_config;
	spi_data_order_config_t spi_data_order_config;
	spi_mode_config_t spi_mode_config;
	spi_polarity_config_t spi_polarity_config;
	spi_chpa_config_t spi_chpa_config;
	spi_prescaller_config_t spi_prescaller_config;

}spi_config_t;

typedef struct{
	en_dioPortsType spi_slave_port;
	en_dioPinsType	spi_slave_pin;
}spi_slave_config_t;


/*
*   -Description-
*-this function init the SPI peripheral
*
*   -Input parameters -
*-1-it holds SPI configuration  (spi_config_t *spi_config)
*
*   -Return-
*   SPIErrorsType
*
*   -Return cases-
*-1- (SPI_E_NOK) if there is something wrong
*-2- (SPI_E_OK) otherwise
*
*/
SPIErrorsType spi_init(spi_config_t *spi_config);

/*
*   -Description-
*-this function init ce pin that will be used to select the slave
*
*   -Input parameters -
*-1-it holds SPI slave pin configuration  (spi_slave_config_t *spi_slave_config)
*
*   -Return-
*   SPIErrorsType
*
*   -Return cases-
*-1- (SPI_E_NOK) if there is something wrong
*-2- (SPI_E_OK) otherwise
*
*/
SPIErrorsType spi_slave_CE_init(spi_slave_config_t *spi_slave_config);

/*
*   -Description-
*-this function used to write just one byte and discard received date
*
*   -Input parameters -
*-1-it holds the transmited data  (uint8_t *data)
*
*   -Return-
*   SPIErrorsType
*
*   -Return cases-
*-1- (SPI_E_NOK) if there is something wrong
*-2- (SPI_E_OK) otherwise
*
*/
SPIErrorsType spi_write(uint8_t *data);
/*
*   -Description-
*-this function used to read just one byte and transmit dummy date
*
*   -output parameters -
*-1-it holds the received data  (uint8_t *data)
*
*   -Return-
*   SPIErrorsType
*
*   -Return cases-
*-1- (SPI_E_NOK) if there is something wrong
*-2- (SPI_E_OK) otherwise
*
*/
SPIErrorsType spi_read(uint8_t *data);

/*
*   -Description-
*-this function used to read just one byte and write one byte 
*
*   -Input parameters -
*-1-it holds the data wants to transmit (uint8_t *transmited_data)
*   -Output parameters -
*-1-it holds the received data  (uint8_t *recieved_data)
*
*   -Return-
*   SPIErrorsType
*
*   -Return cases-
*-1- (SPI_E_NOK) if there is something wrong
*-2- (SPI_E_OK) otherwise
*
*/
SPIErrorsType spi_write_read(uint8_t *transmited_data, uint8_t *recieved_data);


/*
*   -Description-
*-this function used to disable specific slave
*
*   -Input parameters -
*-1-it holds SPI slave pin configuration  (spi_slave_config_t *spi_slave_config)
*
*   -Return-
*   SPIErrorsType
*
*   -Return cases-
*-1- (SPI_E_NOK) if there is something wrong
*-2- (SPI_E_OK) otherwise
*
*/
SPIErrorsType spi_disable_slave(spi_slave_config_t *spi_slave_config);

/*
*   -Description-
*-this function used to enable specific slave
*
*   -Input parameters -
*-1-it holds SPI slave pin configuration  (spi_slave_config_t *spi_slave_config)
*
*   -Return-
*   SPIErrorsType
*
*   -Return cases-
*-1- (SPI_E_NOK) if there is something wrong
*-2- (SPI_E_OK) otherwise
*
*/
SPIErrorsType spi_enable_slave(spi_slave_config_t *spi_slave_config);
#endif /* SPI_H_ */