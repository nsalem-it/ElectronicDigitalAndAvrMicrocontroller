/*
 * USART_Driver_AttinyAVR_Serie0.cpp
 *
 * Created: 2022-03-06 09:37:33
 * Author : Nasreddine Salem
 * Email :  Nasreddine.salem@gmail.com
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/delay.h>

static int USART0_printChar(char character, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(USART0_printChar, NULL,_FDEV_SETUP_WRITE);

#define F_CPU  (3333333UL)         /* using default clock 3,3MHz for Attiny1604*/
#define USART0_BAUD_RATE(BAUD_RATE)     ((float)(64 * 3333333 / (16 * (float)BAUD_RATE)) + 0.5) // macro to calculate BAUD Rate

char String[]="Hello world!!";    //String[] is in fact an

static void USART0_sendChar(char data)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART0.TXDATAL = data;
}

unsigned char USART0_Receive()
{
	while(!(USART0.STATUS & USART_RXCIF_bm))
	{
	}
	return USART0.RXDATAL;
}


static void USART0_putstring(char* StringPtr)
{
	
	while(*StringPtr != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		USART0_sendChar(*StringPtr);    //Using the simple send function we send one char at a time
	StringPtr++;}        //We increment the pointer so we can read the next char
	
}
static int USART0_printChar(char character, FILE *stream)
{
	USART0_sendChar(character);
	return 0;
}


//static FILE USART_stream; //= FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

static void USART0_Init(void)
{
	PORTB.DIRSET |=(1<<PIN2_bp);   /* set pin 2 of PORT C (TXd) as output*/
	PORTB.DIRSET &=~(1<<PIN3_bp);   /* set pin 3 of PORT C (RXd) as input*/
	
	USART0.BAUD = (uint16_t)(USART0_BAUD_RATE(9600));   /* set the baud rate*/
	
	USART0.CTRLC |= USART_CHSIZE_8BIT_gc; // set size of data
	USART0.CTRLB |= (1<<USART_TXEN_bp); // enable TX
	USART0.CTRLB |= (1<<USART_RXEN_bp); // enable RX

		
}


int main(void)
{
	
//	FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);
//	stdout = &USART_stream;
	uint8_t count = 0;
	USART0_Init();
	
	while (1)
	{
		//USART0_sendChar(USART0_Receive()); //loop back call
		//USART0_putstring("Hello World!\r\n");
		for (uint8_t i; i<5 ; i++){
		printf("\Counter value is: %u\r\n", count++);
		_delay_ms(500);
		}
	}
}

