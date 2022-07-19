/*
 * ADC_With_PWM.c
 *
 * Created: 2022-03-10 14:56:26
 * Author : Nasreddine Salem
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART_irq.h"
#define F_CPU  (3333333UL)         /* using default clock 3,3MHz for Attiny1604*/
#define USART0_BAUD_RATE(BAUD_RATE)     ((float)(64 * 3333333 / (16 * (float)BAUD_RATE)) + 0.5) // macro to calculate BAUD Rate

#define  LED_ON PORTA.OUT |=(1<<PIN5_bp) // set PIN5 as HIGH
#define  LED_OFF PORTA.OUT &=~(1<<PIN5_bp) // set PIN5 as LOW
#define  LED_TOGGLE PORTA.OUTTGL |=(1<<PIN5_bp)  // TOGLLE PIN5

volatile uint8_t adc_result = 0;
volatile uint8_t sendFlags = 0;
volatile uint32_t accumulator = 0;
volatile uint16_t average = 0;
volatile uint8_t samples = 0;
ISR(TCA0_CMP0_vect)
{
	LED_ON;
	TCA0.SINGLE.INTFLAGS |=(1<<TCA_SINGLE_CMP0EN_bp);
}

ISR(TCA0_CMP1_vect)
{
	LED_OFF;
	TCA0.SINGLE.INTFLAGS |=(1<<TCA_SINGLE_CMP1EN_bp);
}

ISR(RTC_CNT_vect)
{
	ADC_start_conversion();
	/* Overflow interrupt flag has to be cleared manually */
	RTC.INTFLAGS = RTC_OVF_bm;
	
}

ISR(ADC0_RESRDY_vect)
{
	ADC_get_result();
	uint16_t dutty = adc_result;
	TCA0.SINGLE.CMP1 = dutty;
	accumulator +=dutty;
	samples++;
	if (samples == 100)
	{
		average = accumulator / 100;
		samples = 0;
		accumulator = 0;
		sendFlags=1;
		ADC0.INTFLAGS = ADC_RESRDY_bm; // reset flags 
	}
				
}

void Timer_Frenquency_1s(void)
{
	RTC.CTRLA |=(1<<RTC_RTCEN_bp); //ENABLE RTC
	RTC.CTRLA |=(RTC_PRESCALER_DIV32_gc); //set prescalar to 32 to get 1 KHz periode
	RTC.PER = 0x3E8; // set periode to 1000 to get 1 seconds 
	RTC.INTCTRL |=(1<<RTC_OVF_bp); // Enable overflow interrupt
	
	
}

void PWD_init(void)
{
	TCA0.SINGLE.CTRLA |=TCA_SINGLE_CLKSEL_DIV1_gc;	//NO prescaler for clock
	TCA0.SINGLE.CTRLB |=TCA_SINGLE_WGMODE_FRQ_gc;  // (0x01<<1) enable a timer frequency mode
	TCA0.SINGLE.CTRLB |=(1<<TCA_SINGLE_CMP0EN_bp) | (1<<TCA_SINGLE_CMP1EN_bp); // Enable the wave after compare in register CMP0 and CMP1 output 0 WO0 under the pin 3 (PA5)
	//fFRQ = fCLK_PER / 2N (CMP0+1) equation to calculate frequency depends on CMP0
	TCA0.SINGLE.CTRLA |=(1<<TCA_SINGLE_ENABLE_bp); // ENBALE TIMER COUNTER 
	TCA0.SINGLE.INTCTRL |=(1<<TCA_SINGLE_CMP0EN_bp) | (1<<TCA_SINGLE_CMP1EN_bp); // Enable the channel compare 0 and 1
	TCA0.SINGLE.CMP0 = 1000; // set initial period
	TCA0.SINGLE.CMP1 = 100; // set initial dutty
	
}
void ADC_init(void)
{
	ADC0.CTRLA |= (1<<ADC_ENABLE_bp); //Enable ADC
	//ADC0.CTRLA |= (1<<ADC_FREERUN_bp); //ACD in mode free run
	ADC0.MUXPOS |= ADC_MUXPOS_AIN1_gc; //Chose ADC input 1 AIN1 in the pin PA1 (11)
	ADC0.INTCTRL |=(1<<ADC_RESRDY_bp); //Result Ready Interrupt Enable
	ADC0.CTRLC |=(ADC_PRESC_DIV32_gc); // ADC Prescaler
	ADC0.CTRLC |=(ADC_REFSEL_VDDREF_gc); // set VDD as reference voltage
	
}
//ADC Functions
void ADC_start_conversion(){
	ADC0.COMMAND = (1<<ADC_STCONV_bp); //Set start conversion enable mask bit
}
void ADC_get_result(){
	adc_result = ADC0.RESL;
}

int main(void)
{
	PORTA.DIR |=(1<<PIN5_bp);// set DIR PIN4 as OUTPUT for LED
	ADC_init();
	PWD_init();
	Timer_Frenquency_1s();
		
	USART_init();
	sei();
	printf("Hello World!\r\n");
	while (1)
	{
		if(sendFlags){
			printf("The value is : %u \r\n",adc_result);
			sendFlags=0;
		}
		
	}
}

