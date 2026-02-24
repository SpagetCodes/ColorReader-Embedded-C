/*
 * rgb_reader_i2c.c
 *
 * Created: 2026-02-18 12:55:22
 *  Author: alexa
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include "thesauce.h"
#include "rgb_reader_i2c.h"
#include "display_i2c.h"
#define RGBTCS34725_ADDR 0x29


// /*  drivers i2c protokoll*/
// 
// void i2c_write(uint8_t data){
//     TWDR = data;
//     TWCR = (1 << TWINT) | (1 << TWEN);
//     while ((TWCR & (1 << TWINT))  == 0);
// }
// /*******************************************************/
// void i2c_start(void){
//     TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
//     while ((TWCR & (1 << TWINT)) == 0);
// }
// /*******************************************************/
// void i2c_stop(){
//     TWCR = (1 << TWINT) | (1<<TWEN) |(1<<TWSTO);
// }
// /*******************************************************/
// void i2c_init(void){
//     TWSR=0x00; //* set prescaler bits to zero. */
//     TWBR=12; /* SCL frequency is 50Khz for XTAL = 16Mhz processor. */
//     TWCR=0x04;/* Enable the TWI module. */
// }
// /*******************************************************/

uint8_t i2c_read(uint8_t ack_flag){
        if (ack_flag == 0){
        TWCR = (1 << TWINT) | (1 << TWEN) | (1<<TWEA);
        }
        else{ /*only one bit*/
             TWCR = (1 << TWINT) | (1 << TWEN) ;
        }
      while ((TWCR & (1 << TWINT))  == 0);
      return TWDR; 
}
/************************************************************************/

uint16_t StartRGB(uint16_t *RGBC_value){
  i2c_init();
   /*Power on and enbles ADC (colorread)*/
  i2c_start();
  i2c_write(RGBTCS34725_ADDR << 1); /* Send Address + Write (0) */
  i2c_write(0x80 | 0x00);           /* Command bit (0x80) + Enable Register (0x00) */
  i2c_write(0x03);                  /* Power ON (PON) | ADC Enable (AEN) */
  i2c_stop();
   
  _delay_ms(614); /* need for the sensor see pdf data sheet TCS34725*/ 
  /*Should be 3 ms but the sensor has a bug that it will default to the highest reading cycle  so it needs 614 to complete the scan correctly*/ 
  
  /*Set up read color sequence */
 i2c_start();
 i2c_write(RGBTCS34725_ADDR << 1);
 
 i2c_write(0xA0 | 0x14);  /* Table 14 ADC channel */
 
 /*repeated to start read mode*/
 i2c_start();
 i2c_write((RGBTCS34725_ADDR << 1) | 1);
 
 /*Read 8 bit High &  low (C,R,G,B)*/
 
 uint8_t c_l = i2c_read(0);
 uint8_t c_h = i2c_read(0);
 
 uint8_t r_l = i2c_read(0);
 uint8_t r_h = i2c_read(0);
 
 uint8_t g_l = i2c_read(0);
 uint8_t g_h = i2c_read(0);
 
 uint8_t b_l = i2c_read(0);
 uint8_t b_h = i2c_read(1);
 
 i2c_stop();
 
 *RGBC_value = ((uint16_t)c_h << 8) | c_l; // Clear Bit shift för att få in hela värdet i en variabel
 *(RGBC_value+1) = ((uint16_t)r_h << 8) | r_l; // Red  Bit shift för att få in hela värdet i en variabel
 *(RGBC_value+2) = ((uint16_t)g_h << 8) | g_l; // Green Bit shift för att få in hela värdet i en variabel
 *(RGBC_value+3) = ((uint16_t)b_h << 8) | b_l; // Blue Bit shift för att få in hela värdet i en variabel
 
}   

/************************************************************************/