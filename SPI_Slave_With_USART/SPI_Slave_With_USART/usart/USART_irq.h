/*
 * USART_irq.h
 *
 * Created: 2022-03-07 14:08:04
 *  Author: Hi
 */ 


#ifndef USART_IRQ_H_
#define USART_IRQ_H_


/*Prototype*/
void USART_sendChar(char data);
void uart_puts(const char *s);
unsigned char USART_Receive();
void USART_putstring(char* StringPtr);
void USART_init(void);


#endif /* USART_IRQ_H_ */