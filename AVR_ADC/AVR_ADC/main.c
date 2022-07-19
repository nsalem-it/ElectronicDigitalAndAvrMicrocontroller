/*
 * AVR_ADC.c
 *
 * Created: 2022-03-08 12:54:22
 * Author : Nasreddine Salem
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

 #define  LED_ON PORTA.OUT |=(1<<PIN5_bp) // set PIN5 as HIGH
 #define  LED_TOGGLE PORTA.OUTTGL |=(1<<PIN5_bp)  // TOGLLE PIN5

ISR(ADC0_RESRDY_vect)
{
	LED_TOGGLE;
}

void ADC_init(void)
{
	ADC0.CTRLA |= (1<<ADC_ENABLE_bp); //Enable ADC
	ADC0.CTRLA |= (1<<ADC_FREERUN_bp); //ACD in mode free run
	ADC0.MUXPOS |= ADC_MUXPOS_AIN1_gc; //Chose ADC input 1 AIN1 in the pin PA1 (11)
	ADC0.INTCTRL |=(1<<ADC_RESRDY_bp); //Result Ready Interrupt Enable
	ADC0.CTRLC |=(ADC_PRESC_DIV32_gc); // ADC Prescaler
	ADC0.CTRLC |=(ADC_REFSEL_VDDREF_gc);
	ADC0.COMMAND |=(1<<ADC_STCONV_bp); // start commande
	sei();
	
}

int main(void)
{
    PORTA.DIR |=(1<<PIN5_bp);// set DIR PIN4 as OUTPUT for LED
	ADC_init();
    while (1) 
    {
    }
}

