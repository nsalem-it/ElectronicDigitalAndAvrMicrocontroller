/*
 * SPI_Master.c
 *
 * Created: 2022-04-17 15:56:51
 * Author : Nasreddine
 */ 

#include <avr/io.h>
#include "spi.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdbool.h>

bool flag = false;

ISR(PORTA_PORT_vect)
{
	// user code here
	if (PORTA.IN  & (1<<PIN5_bp)) //  switch is off
	{
	}
	else
	{
		flag =true;
	}
}
int main(void)
{

	PORTA.DIR &=~(1<<PIN5_bp); // set pin 3 as input
	PORTA.PIN5CTRL |=(1<<PORT_PULLUPEN_bp); // pull up Resistance
	PORTA.PIN5CTRL |= PORT_ISC_LEVEL_gc; // configure Interrupt low for pin 3
	spi_init_As_Master();
	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sei();
    while (1) 
    {
		sleep_mode();

		if (flag)
		{
			spi_write_string("Hello Attiny1614 \n");
			flag =false;			
		}
    }
}

