/*
 * ssd1306.c
 *
 * Created: 2022-03-26 13:30:00
 *  Author: Hi
 *
/*--------------------------------------------------------
 * I2C SSD1306 128x64 OLED support routines for ATtiny1604
 *
 * Base on ATTiny85 source code available at: https://bitbucket.org/tinusaur/ssd1306xled
 */

#ifndef SSD1306_ATtiny1614_h
#define SSD1306_ATtiny1614_h

#include <avr/sleep.h>

//---------------------- ATtiny1614 ----------------------

#define SSD1306_SCL   PIN0_bm   // ----> [SCL]  Pin 3 on the SSD1306 display board
#define SSD1306_SDA   PIN1_bm   // ----> [SDA]  Pin 4 on the SSD1306 display board
#define SSD1306_SA    0x78  // Slave address

#define DIGITAL_WRITE_HIGH(PORT) PORTB.OUTSET = PORT
#define DIGITAL_WRITE_LOW(PORT) PORTB.OUTCLR = PORT

//----------------- Forward references -------------------

void ssd1306_init(void);
void ssd1306_xfer_start(void);
void ssd1306_xfer_stop(void);
void ssd1306_send_byte(uint8_t byte);
void ssd1306_send_command(uint8_t command);
void ssd1306_send_data_start(void);
void ssd1306_send_data_stop(void);
void ssd1306_setpos(uint8_t x, uint8_t y);
void ssd1306_fillscreen(uint8_t fill_Data);
void ssd1306_char_f6x8(uint8_t x, uint8_t y, const char ch[]);
void ssd1306_char_font6x8(char ch) ;
void ssd1306_string_font6x8(char *s); 
void ssd1306_char_f8x16(uint8_t x, uint8_t y, const char ch[]);
void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]);
//Extra routines commonly used in ATtiny Arcade Games
void ssd1306_beep(uint8_t pin, int bCount, int bDelay);
void ssd1306_sleep();

//----------------- Functions -------------------

// Some code based on "IIC_wtihout_ACK" by http://www.14blog.com/archives/1358

const uint8_t ssd1306_init_sequence [] PROGMEM = {  // Initialization Sequence
  0xAE,     // Display OFF (sleep mode)
  0x20, 0b00,   // Set Memory Addressing Mode
          // 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
          // 10=Page Addressing Mode (RESET); 11=Invalid
  0xB0,     // Set Page Start Address for Page Addressing Mode, 0-7
  0xC8,     // Set COM Output Scan Direction
  0x00,     // ---set low column address
  0x10,     // ---set high column address
  0x40,     // --set start line address
  0x81, 0x3F,   // Set contrast control register
  0xA1,     // Set Segment Re-map. A0=address mapped; A1=address 127 mapped. 
  0xA6,     // Set display mode. A6=Normal; A7=Inverse
  0xA8, 0x3F,   // Set multiplex ratio(1 to 64)
  0xA4,     // Output RAM to Display
          // 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
  0xD3, 0x00,   // Set display offset. 00 = no offset
  0xD5,     // --set display clock divide ratio/oscillator frequency
  0xF0,     // --set divide ratio
  0xD9, 0x22,   // Set pre-charge period
  0xDA, 0x12,   // Set com pins hardware configuration    
  0xDB,     // --set vcomh
  0x20,     // 0x20,0.77xVcc
  0x8D, 0x14,   // Set DC-DC enable
  0xAF      // Display ON in normal mode
  
};

void ssd1306_init(void)
{
  PORTB.DIRSET = SSD1306_SDA; // Set port as output
  PORTB.DIRSET = SSD1306_SCL; // Set port as output
  
  for (uint8_t i = 0; i < sizeof (ssd1306_init_sequence); i++) {
    ssd1306_send_command(pgm_read_byte(&ssd1306_init_sequence[i]));
  }
}

void ssd1306_xfer_start(void)
{
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
}

void ssd1306_xfer_stop(void)
{
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
}

void ssd1306_send_byte(uint8_t byte)
{
  uint8_t i;
  for(i=0; i<8; i++)
  {
    if((byte << i) & 0x80)
      DIGITAL_WRITE_HIGH(SSD1306_SDA);
    else
      DIGITAL_WRITE_LOW(SSD1306_SDA);
    
    DIGITAL_WRITE_HIGH(SSD1306_SCL);
    DIGITAL_WRITE_LOW(SSD1306_SCL);
  }
  DIGITAL_WRITE_HIGH(SSD1306_SDA);
  DIGITAL_WRITE_HIGH(SSD1306_SCL);
  DIGITAL_WRITE_LOW(SSD1306_SCL);
}

void ssd1306_send_command(uint8_t command)
{
  ssd1306_xfer_start();
  ssd1306_send_byte(SSD1306_SA);  // Slave address, SA0=0
  ssd1306_send_byte(0x00);  // write command
  ssd1306_send_byte(command);
  ssd1306_xfer_stop();
}

void ssd1306_send_data_start(void)
{
  ssd1306_xfer_start();
  ssd1306_send_byte(SSD1306_SA);
  ssd1306_send_byte(0x40);  //write data
}

void ssd1306_send_data_stop(void)
{
  ssd1306_xfer_stop();
}

void ssd1306_setpos(uint8_t x, uint8_t y)
{
  //X = 0..127, Y = 0..7
  ssd1306_xfer_start();
  ssd1306_send_byte(SSD1306_SA);  //Slave address,SA0=0
  ssd1306_send_byte(0x00);  //write command

  ssd1306_send_byte(0xb0+y);             //Set Page Start Address for Page Addressing Mode 10110xxx
  ssd1306_send_byte(((x&0xf0)>>4)|0x10); //Set Lower Column Start Address for Page Addressing Mode 0000xxxx
  ssd1306_send_byte((x&0x0f)|0x01);      //Set Higher Column Start Address for Page Addressing Mode 0001xxxx

  ssd1306_xfer_stop();
}

void ssd1306_fillscreen(uint8_t fill_Data)
{
  uint8_t m,n;
  for(m=0;m<8;m++)
  {
    ssd1306_send_command(0xb0+m);  //Set Page Start Address for Page Addressing Mode 10110xxx
    ssd1306_send_command(0x00);    //Set Lower Column Start Address for Page Addressing Mode 0000xxxx
    ssd1306_send_command(0x10);    //Set Higher Column Start Address for Page Addressing Mode 0001xxxx
    ssd1306_send_data_start();
    for(n=0;n<128;n++)
    {
      ssd1306_send_byte(fill_Data);
    }
    ssd1306_send_data_stop();
  }
}

void ssd1306_char_f6x8(uint8_t x, uint8_t y, const char ch[])
{
  uint8_t c,i,j=0;
  while(ch[j] != '\0')
  {
    c = ch[j] - 32;
    /*
    if (c >0) c = c - 12;
    if (c >15) c = c - 6;
    if (c>40) c=c-9;
    */
    if(x>126)
    {
      x=0;
      y++;
    }
    ssd1306_setpos(x,y);
    ssd1306_send_data_start();
    for(i=0;i<6;i++)
    {
      ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font6x8[c*6+i]));
    }
    ssd1306_send_data_stop();
    x += 6;
    j++;
  }
}

void ssd1306_char_font6x8(char ch) 
{
  uint8_t i; 
  uint8_t c = ch - 32;
  ssd1306_send_data_start();
  for (i= 0; i < 6; i++)
  {
    ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font6x8[c * 6 + i]));
  }
  ssd1306_send_data_stop();
}

void ssd1306_string_font6x8(char *s) 
{
  while (*s) 
  {
    ssd1306_char_font6x8(*s++);
  }
}

#ifdef ssd1306xled_font8x16

void ssd1306_char_f8x16(uint8_t x, uint8_t y, const char ch[])
{
  uint8_t c, j, i = 0;
  while (ch[j] != '\0')
  {
    c = ch[j] - 32;
    if (x > 120)
    {
      x = 0;
      y++;
    }
    ssd1306_setpos(x, y);
    ssd1306_send_data_start();
    for (i = 0; i < 8; i++)
    {
      ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font8x16[c * 16 + i]));
    }
    ssd1306_send_data_stop();
    ssd1306_setpos(x, y + 1);
    ssd1306_send_data_start();
    for (i = 0; i < 8; i++)
    {
      ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font8x16[c * 16 + i + 8]));
    }
    ssd1306_send_data_stop();
    x += 8;
    j++;
  }
}

#endif

void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[])
{
  uint16_t j = 0;
  uint8_t y, x;
  if (y1 % 8 == 0) y = y1 / 8;
  else y = y1 / 8 + 1;
  for (y = y0; y < y1; y++)
  {
    ssd1306_setpos(x0,y);
    ssd1306_send_data_start();
    for (x = x0; x < x1; x++)
    {
      ssd1306_send_byte(pgm_read_byte(&bitmap[j++]));
    }
    ssd1306_send_data_stop();
  }
}

//--------------------- Extra -------------------------------

//Simple tone generator
void ssd1306_beep(uint8_t pin, int bCount, int bDelay)
{
  for (int i = 0; i<=bCount; i++) 
  {
    digitalWrite(pin,HIGH);
    for(int i2=0; i2<bDelay; i2++) 
    {
      __asm__("nop\n\t");
    }
    digitalWrite(pin,LOW);
    for(int i2=0; i2<bDelay; i2++)
    {
      __asm__("nop\n\t");
    }
  }
}

//Shut down OLED and put ATtiny to sleep
//Will wake up when LEFT button is pressed
void ssd1306_sleep() 
{
  ssd1306_fillscreen(0x00);
  ssd1306_send_command(0xAE);
  //cbi(ADCSRA,ADEN);                   // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // sleep mode is set here
  sleep_enable();
  sleep_mode();                         // System actually sleeps here
  sleep_disable();                      // System continues execution here when watchdog timed out
  //sbi(ADCSRA,ADEN);                   // switch Analog to Digitalconverter ON
  ssd1306_send_command(0xAF);
}
#endif