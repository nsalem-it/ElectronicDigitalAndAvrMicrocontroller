/*
 * OledDisplay124x64.c
 *
 * Created: 2022-03-26 13:29:23
 * Author : Hi
 */ 

#include <avr/io.h>
#include "ssd1306.h"
#include "font6x8.h"
int main(void)
{
	  ssd1306_init();
	  ssd1306_fillscreen(0x00);
	  ssd1306_char_f6x8(0, 0, "--------------------");
	  ssd1306_char_f6x8(0, 1, "Hello world");
    while (1) 
    {
    }
}

