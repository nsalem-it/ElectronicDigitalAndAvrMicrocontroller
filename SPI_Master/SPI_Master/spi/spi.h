/*
spi lib 0x01

copyright (c) Nasreddine Salem 2022

References:
  -  This library is based upon SPI avr lib by Stefan Engelke
     http://www.tinkerer.eu/AVRLib/SPI

Please refer to LICENSE file for licensing information.
*/


#ifndef _SPI_H_
#define _SPI_H_

#include <avr/io.h>

//spi ports
#define SPI_DIR PORTA.DIRSET
#define SPI_PORT PORTA
#define SPI_MISO PIN2_bm
#define SPI_MOSI PIN1_bm
#define SPI_SCK PIN3_bm
#define SPI_SS PIN4_bm

extern void spi_init_As_Master(void);
extern unsigned char spi_writereadbyte(unsigned char data);
extern void spi_send( unsigned char send_data );
extern void spi_write_string(char *data);
extern void slaveSelect(void);
extern void slaveDeselect(void);
#endif
