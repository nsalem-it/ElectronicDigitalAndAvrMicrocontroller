/*
 * Timer_Using_FunctionCompare_CRM.cpp
 *
 * Created: 2022-02-28 17:02:43
 * Author : Nasreddine Salem
 */ 

 #include <avr/io.h>
 #include <avr/delay.h>
 #include <avr/interrupt.h>

 #define F_CPU  3333333UL

 #define  LED_ON PORTA.OUT |=(1<<PIN5_bp) // set PIN5 as HIGH
 #define  LED_OFF PORTA.OUT &=~(1<<PIN5_bp) // set PIN5 as LOW
 #define  LED_TOGGLE PORTA.OUTTGL |=(1<<PIN5_bp)  // TOGLLE PIN5

 ISR(TCA0_CMP0_vect)
 {
 LED_TOGGLE;
 TCA0.SINGLE.INTFLAGS |=(1<<TCA_SINGLE_CMP0EN_bp);
 }
 
 void Timer_Frenquency(uint8_t frenquency)
 {
	 TCA0.SINGLE.CTRLB |=TCA_SINGLE_WGMODE_FRQ_gc;  // (0x01<<1) enable a timer frequency mode
	 TCA0.SINGLE.CTRLB |=(1<<TCA_SINGLE_CMP0EN_bp); // Enable the wave output 0 WO0 under the pin 3 (PA5)
	 //fFRQ = fCLK_PER / 2N (CMP0+1) equation to calculate frequency depends on CMP0
	 TCA0.SINGLE.CMP0 =(F_CPU/(frenquency*2*256)-1);
	 
 }
 

 int main(void)
 {
 
 PORTA.DIR |=(1<<PIN5_bp);// set DIR PIN4 as OUTPUT
 TCA0.SINGLE.CTRLA |=(1<<TCA_SINGLE_ENABLE_bp); // ENBALE TIMER COUNTER 0
 TCA0.SINGLE.CTRLA |=TCA_SINGLE_CLKSEL_DIV256_gc; 
 //set prescaler for clock
 TCA0.SINGLE.INTCTRL |=(1<<TCA_SINGLE_CMP0EN_bp); // Enable the inerrupt for comapare register 0
 Timer_Frenquency(1);
 sei();
 
 /* Replace with your application code */
 while (1)
 {
 }
 
 }
