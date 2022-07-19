/*
 * Blink_LED_With_TIMER_CRM.cpp
 *
 * Created: 2022-02-25 18:11:15
 * Author : Nasreddine Salem
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define F_CPU  3333333UL;

#define  LED_ON PORTA.OUT |=(1<<PIN4_bp);// set PIN4 as HIGH
#define  LED_OFF PORTA.OUT &=~(1<<PIN4_bp);// set PIN4 as LOW
#define  LED_TOGGLE PORTB.OUTTGL |=(1<<PIN_bp); // TOGLLE PIN4

ISR(TCA0_OVF_vect)
{
	LED_TOGGLE;
	TCA0.SINGLE.INTFLAGS |=(1<<TCA_SINGLE_OVF_bp);
	
}

int main(void)
{
	PORTA.DIR |=(1<<PIN4_bp);// set DIR PIN4 as OUTPUT
	TCA0.SINGLE.CTRLA |=(1<<TCA_SINGLE_ENABLE_bp); // ENABLE TIMER TCA0
	TCA0.SINGLE.CTRLA |=TCA_SINGLE_CLKSEL_DIV16_gc; // Selection CLOCK Presacler
	
	TCA0.SINGLE.INTCTRL |=(1<<TCA_SINGLE_OVF_bp); // Enable OverFlow Interrupt
	
	sei(); // Trigger service interrupt
	while (1)
	{
	}
	
}
