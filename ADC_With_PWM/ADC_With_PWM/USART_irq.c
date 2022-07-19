 /*
 * USART_Driver_AVR_Serie0.c
 *
 * Created: 2022-03-07 13:31:29
 * Author : Hi
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/delay.h>
#include "USART_irq.h"

#define F_CPU  (3333333UL)         /* using default clock 3,3MHz for Attiny1604*/
#define USART0_BAUD_RATE(BAUD_RATE)     ((float)(64 * 3333333 / (16 * (float)BAUD_RATE)) + 0.5) // macro to calculate BAUD Rate


ISR(USART0_RXC_vect)
{
		while(!(USART0.STATUS & USART_RXCIF_bm))
		{
		}
		return USART0.RXDATAL;
}
void USART_sendChar(char data)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART0.TXDATAL = data;
}

unsigned char USART_Receive()
{
	while(!(USART0.STATUS & USART_RXCIF_bm))
	{
	}
	return USART0.RXDATAL;
}


void USART_putstring(char* StringPtr)
{
	
	while(*StringPtr != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		USART_sendChar(*StringPtr);    //Using the simple send function we send one char at a time
	StringPtr++;}        //We increment the pointer so we can read the next char
	
}
int USART_printChar(char character, FILE *stream)
{
	USART_sendChar(character);
	return 0;
}


static FILE USART_stream = FDEV_SETUP_STREAM(USART_printChar, NULL,_FDEV_SETUP_WRITE);

void USART_init(void)
{
	PORTB.DIRSET |=(1<<PIN2_bp);   /* set pin 2 of PORT C (TXd) as output*/
	PORTB.DIRSET &=~(1<<PIN3_bp);   /* set pin 3 of PORT C (RXd) as input*/
	
	USART0.BAUD = (uint16_t)(USART0_BAUD_RATE(9600));   /* set the baud rate*/
	
	USART0.CTRLC |= USART_CHSIZE_8BIT_gc; // set size of data
	USART0.CTRLB |= (1<<USART_TXEN_bp); // enable TX
	USART0.CTRLB |= (1<<USART_RXEN_bp); // enable RX
	
	USART0.CTRLA |= (1<<USART_RXCIE_bp); // enable interrupt for RX complete

	stdout = &USART_stream;
	
}
