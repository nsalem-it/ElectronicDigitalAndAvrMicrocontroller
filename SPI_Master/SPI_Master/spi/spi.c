/*
spi lib 0x01

copyright (c) Nasreddine Salem, 2022

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/



#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "spi.h"

/*
 * spi initialize
 */
void spi_init_As_Master(void) {
	
	SPI_DIR |= SPI_MOSI; /* Set MOSI pin direction to output */
	SPI_DIR &= ~(SPI_MISO); /* Set MISO pin direction to input */
	SPI_DIR |= SPI_SCK; /* Set SCK pin direction to output */
	SPI_DIR |= SPI_SS; /* Set SS pin direction to output */
	
 SPI0.CTRLA = SPI_CLK2X_bm /* Enable double-speed */
			| SPI_DORD_bm /* LSB is transmitted first */
			| SPI_ENABLE_bm /* Enable module */
			| SPI_MASTER_bm /* SPI module in Master mode */
			| SPI_PRESC_DIV64_gc; /* System Clock divided by 64  frequence de communication 3333333/64 */
}


/*
 * spi write one byte and read it back
 */

void slaveSelect(void)
{
	PORTA.OUT &= ~SPI_SS; // Set SS pin value to LOW
}
void slaveDeselect(void)
{
	PORTA.OUT |= SPI_SS; // Set SS pin value to HIGH
}
unsigned char spi_writereadbyte(unsigned char data) {
    SPI0.DATA = data;
 while (!(SPI0.INTFLAGS & SPI_IF_bm)) /* waits until data is exchanged*/
 {
	 ;
 }
 return SPI0.DATA;
}

void spi_send( unsigned char send_data )
{
    SPI0.DATA = send_data;
    while (!(SPI0.INTFLAGS & SPI_IF_bm)) /* waits until data is exchanged*/
    {
	    ;
    }
}

void spi_write_string(char* data)
{
	while(*data !=0x00){
		slaveSelect();
		spi_send(*data);
		_delay_ms(10);
		slaveDeselect();
		data++;
	}
}

