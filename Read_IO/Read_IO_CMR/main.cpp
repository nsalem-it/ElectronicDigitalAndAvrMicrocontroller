/*
 * Read_IO_CMR.cpp
 *
 * Created: 2022-02-22 18:11:31
 * Author : Nasreddine Salem
 */ 

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define LED_ON PORTA.OUT |=(1<<PIN4_bp); // Set pin4 output as high
#define LED_OFF PORTA.OUT &=~(1<<PIN4_bp); // Set pin4 output as low

ISR(PORTA_PORT_vect)
{
	// user code here
				if (PORTA.IN  & (1<<PIN5_bp)) //  switch is off
				{
					LED_OFF;
				}
				else
				{
					LED_ON;
				}
}

int main(void)
{
    /* Replace with your application code */
	
	PORTA.DIR |=(1<<PIN4_bp); // set pin 2 as output
	PORTA.DIR &=~(1<<PIN5_bp); // set pin 3 as input
	PORTA.PIN5CTRL |=(1<<PORT_PULLUPEN_bp); // pull up Resis
	PORTA.PIN5CTRL |= PORT_ISC_LEVEL_gc; // configure Interrupt low for pin 3 
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sei();

    while (1) 
    {			
			sleep_mode();		
    }
}