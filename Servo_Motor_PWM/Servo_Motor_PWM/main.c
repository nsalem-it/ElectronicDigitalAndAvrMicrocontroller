/*
 * Servo_Motor_PWM.c
 *
 * Created: 2022-03-15 18:16:29
 * Author : Nasreddine Salem
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define F_CPU  (3333333UL)         /* using default clock 3,3MHz for Attiny1604*/
#define  LED_ON PORTA.OUT |=(1<<PIN5_bp) // set PIN5 as HIGH
#define  LED_OFF PORTA.OUT &=~(1<<PIN5_bp) // set PIN5 as LOW
#define  LED_TOGGLE PORTA.OUTTGL |=(1<<PIN5_bp)  // TOGLLE PIN5

extern volatile uint16_t periode= 4165; // calcul the periode using formula TOP=Periode= ( 33333333 /50 * 16)-1 = 4165
extern volatile uint16_t dutty = 0;
ISR(TCA0_CMP0_vect)
{
			TCA0.SINGLE.CMP0BUF = 0; // initialisation
			_delay_ms(2000);
			TCA0.SINGLE.CMP0BUF = 81; // 0 degree
			_delay_ms(2000);
			TCA0.SINGLE.CMP0BUF = 280; // 90 degree
			_delay_ms(2000);
			TCA0.SINGLE.CMP0BUF = 470; // 180 degree
			_delay_ms(2000);
			TCA0.SINGLE.CMP0BUF = 280; // 90 degree back
			_delay_ms(2000);
			TCA0.SINGLE.INTFLAGS |=(1<<TCA_SINGLE_CMP0_bp);
}
void  Power_ON(void)
{
	PORTA.DIR |=(1<<PIN5_bp);// set DIR PIN4 as OUTPUT
	LED_ON;
}
void  PWM_init(void)
{
	// Choise the PWM MODE
	TCA0.SINGLE.CTRLB |= TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
	//Set presaclar DIV16 to get 20ms by period
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_CLKSEL_DIV16_gc;
	//ENABLE COMPARE CHANAL CMP0
	TCA0.SINGLE.CTRLB |=(1<< TCA_SINGLE_CMP0EN_bp);
	//ENABLE INTERRUPT FOR CMP0
	TCA0.SINGLE.INTCTRL |=(1<<TCA_SINGLE_CMP0_bp);
	//enable TCA0
	TCA0.SINGLE.CTRLA |=(1<<TCA_SINGLE_ENABLE_bp);
	// set default value periode
	TCA0.SINGLE.PERBUF = periode ;
}

int main(void)
{
   	// set DIR PIN PB0 as OUTPUT
   	PORTB.DIRSET |=(1<<PIN0_bp);
	Power_ON();   
   	PWM_init();
   	sei();
    while (1) 
    {


		           
    }
}

