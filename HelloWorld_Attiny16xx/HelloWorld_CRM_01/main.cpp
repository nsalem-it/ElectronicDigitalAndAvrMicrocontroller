/*
 * HelloWorld_CRM_01.cpp
 *
 * Created: 2022-02-21 18:45:03
 * Author : Nasreddine Salem
 */ 

#include <avr/io.h>
#include <avr/delay.h>

#define F_CPU  3333333UL;

#define  LED_ON PORTA.OUT |=(1<<PIN4_bp);// set PIN4 as HIGH 
#define  LED_OFF PORTA.OUT &=~(1<<PIN4_bp);// set PIN4 as LOW 


int main(void)
{
	
	PORTA.DIR |=(1<<PIN4_bp);// set DIR PIN4 as OUTPUT
	
    /* Replace with your application code */
    while (1) 
    {
		LED_ON;
		_delay_us(3333333);
		LED_OFF;
		_delay_us(3333333);
    }
}

