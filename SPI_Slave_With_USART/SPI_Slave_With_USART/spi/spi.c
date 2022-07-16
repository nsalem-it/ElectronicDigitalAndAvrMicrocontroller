/*
spi lib 0x01

copyright (c) Nasreddine Salem, 2022

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#include "spi.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#define SPI_PORT PORTA
#define SPI_MISO PIN2_bm
#define SPI_MOSI PIN1_bm
#define SPI_SCK PIN3_bm
#define SPI_SS PIN4_bm

/*
 * spi initialize
 */


void spi_init_As_Slave(void) {
	
 PORTA.DIR &= (~PIN1_bm); /* Set MOSI pin direction to input */
 PORTA.DIR |= PIN2_bm; /* Set MISO pin direction to output */
 PORTA.DIR &= (~PIN3_bm); /* Set SCK pin direction to input */
 PORTA.DIR &= (~PIN4_bm); /* Set SS pin direction to input */
 
 SPI0.CTRLA |= SPI_DORD_bm; /* LSB is transmitted first */
SPI0.CTRLA |=SPI_ENABLE_bm; /* Enable module */
SPI0.CTRLA &=(~SPI_MASTER_bm); /* SPI module in Slave mode */
 SPI0.INTCTRL = SPI_IE_bm; /* SPI Interrupt enable */
}

/*
 * spi write one byte and read it back
 */
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
