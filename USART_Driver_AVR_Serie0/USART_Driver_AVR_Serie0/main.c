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


int main(void)
{
	
	uint8_t count = 0;
	USART_init();
	sei();
	while (1)
	{
		USART_sendChar(USART_Receive()); //loop back call
		//USART0_putstring("Hello World!\r\n");
		//for (uint8_t i=0; i<5 ; i++){
		//printf("\Counter value is: %u %u \n", count++,i);
	//	_delay_ms(500);
		//}
	}
}

