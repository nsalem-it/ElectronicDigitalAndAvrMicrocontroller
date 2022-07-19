/*
 * USART_BASE.cpp
 *
 * Created: 2022-03-04 14:59:27
 * Author : Hi
 */ 
#define F_CPU 3333333
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#define  LED_ON PORTA.OUT |=(1<<PIN5_bp) // set PIN5 as HIGH
#define  LED_OFF PORTA.OUT &=~(1<<PIN5_bp) // set PIN5 as HIGH
#define  LED_TOGGLE PORTA.OUTTGL |=(1<<PIN5_bp)  // TOGLLE PIN5

ISR(USART0_RXC_vect)
{
	LED_ON;
	_delay_ms(500);
	USART0.STATUS |= (1<<USART_RXSIF_bp);
	LED_OFF;
}

ISR(USART0_TXC_vect)
{
	LED_ON;
	_delay_ms(500);
	USART0.STATUS |= (1<<USART_TXCIF_bp);
	LED_OFF;
}
void USART_Init(void)
{
	PORTB.DIRSET |=(1<<PIN2_bp);   /* set pin 2 of PORT C (TXd) as output*/
	PORTB.DIRSET &=~(1<<PIN3_bp);   /* set pin 3 of PORT C (RXd) as input*/
	
	USART0.BAUD = (uint16_t)(USART0_BAUD_RATE(9600));   /* set the baud rate*/
	
	USART0.CTRLC |= USART_CHSIZE_8BIT_gc; // set size of data
	USART0.CTRLB |= (1<<USART_TXEN_bp); // enable TX
	USART0.CTRLB |= (1<<USART_RXEN_bp); // enable RX
	
	USART0.CTRLA |=(1<<USART_RXSIE_bp); //Enable START RX receiver interrupt
	USART0.CTRLA |=(1<<USART_TXCIE_bp); //Enable START RX receiver interrupt
	
}

void USART_Transmit(unsigned char c)
{
	while(!(USART0.STATUS & USART_DREIF_bm))
	{
	}
	
	USART0.TXDATAL = c;
}


unsigned char USART_Receive()
{
	while(!(USART0.STATUS & USART_RXCIF_bm))
	{
	}
	return USART0.RXDATAL;
}
int main(void)
{
	PORTA.DIR |=(1<<PIN5_bp);// set DIR PIN4 as OUTPUT for LED
	USART_Init();
	while (1)
	{
		USART_Transmit(USART_Receive());// loop back
		sei();
	}
}



