/*
 * SPI_Slave_With_USART.c
 *
 * Created: 2022-04-19 16:45:51
 * Author : Nasreddine
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include "spi.h"
#include "USART_irq.h"


volatile unsigned char receiveData = 0x00;
volatile unsigned char writeData = 0x00;bool flag = false;

ISR(SPI0_INT_vect)
{
	receiveData = SPI0.DATA;
	SPI0.DATA = writeData;
	SPI0.INTFLAGS = SPI_IF_bm; /* Clear the Interrupt flag by writing 1 */
	flag =true;
}

int main(void)
{
    spi_init_As_Slave();
	USART_init();
	sei();
	
    while (1) 
    {		
		if(flag){
			USART_sendChar(receiveData);
			flag=false;
		}
    }
}

