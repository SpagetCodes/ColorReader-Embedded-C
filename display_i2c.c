/*
 * display_i2c.c
 *
 * Created: 2026-01-20 14:56:55
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
#include "font.h"
#include "fontlarge.h"
#define SSD1306_ADDR 0x3C




/* Oled drivers i2c protokoll*/

void i2c_write(uint8_t data){
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT))  == 0);
}
/*******************************************************/
void i2c_start(void){
    TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
    while ((TWCR & (1 << TWINT)) == 0);
} 
/*******************************************************/
void i2c_stop(){
    TWCR = (1 << TWINT) | (1<<TWEN) |(1<<TWSTO);
}
/*******************************************************/
void i2c_init(void){
    TWSR=0x00; //* set prescaler bits to zero. */
    TWBR=12; /* SCL frequency is 50Khz for XTAL = 16Mhz processor. */
    TWCR=0x04;/* Enable the TWI module. */
}
/************************************************************************/
void SendCommand(uint8_t command){
    i2c_start();
    i2c_write(SSD1306_ADDR << 1);
    i2c_write(0x00);
    i2c_write(command);
    i2c_stop();
}
/************************************************************************/
void SendData(uint8_t data){
    i2c_start();
    i2c_write(SSD1306_ADDR <<1);
    i2c_write(0x40);
    i2c_write(data);
    i2c_stop();
}
/************************************************************************/

void Init_Screen(void){
    _delay_ms(1);
    // Standard initialization sequence for 128x64 OLED
    SendCommand(0xAE); // Display OFF (sleep mode)
    
    SendCommand(0x20); // Set Memory Addressing Mode
    SendCommand(0x00); // 00b, Horizontal Addressing Mode
    
    SendCommand(0xB0); // Set Page Start Address for Page Addressing Mode,0-7
    
    SendCommand(0xC8); // Set COM Output Scan Direction
    
    SendCommand(0x00); // Set low column address
    SendCommand(0x10); // Set high column address
    
    SendCommand(0x40); // Set start line address
    
    SendCommand(0x81); // Set contrast control register
    SendCommand(0xFF); // Contrast value (0-255)
    
    SendCommand(0xA1); // Set Segment Re-map. A0=address mapped; A1=re-mapped
    
    SendCommand(0xA6); // Set Normal display (A7 for inverse)
    
    SendCommand(0xA8); // Set Multiplex Ratio
    SendCommand(0x3F); // 64 MUX (for 128x64)
    
    SendCommand(0xA4); // Output follows RAM content (0xA5 = all pixels on)
    
    SendCommand(0xD3); // Set display offset
    SendCommand(0x00); // No offset
    
    SendCommand(0xD5); // Set display clock divide ratio/oscillator frequency
    SendCommand(0xF0); // Set divide ratio
    
    SendCommand(0xD9); // Set pre-charge period
    SendCommand(0x22);
    
    SendCommand(0xDA); // Set com pins hardware configuration
    SendCommand(0x12);
    
    SendCommand(0xDB); // Set vcomh
    SendCommand(0x20); // 0.77xVcc
    
    // -- IMPORTANT: ENABLE CHARGE PUMP --
    SendCommand(0x8D); // Charge Pump Setting
    SendCommand(0x14); // Enable Charge Pump
    
    SendCommand(0xAF); // Display ON
}

/************************************************************************/
void ClearRam(void){
    for (uint8_t page = 0;page <8;page++){
        SendCommand(0xB0 + page); // sets page
        SendCommand(0x00); //Set lower Column
        SendCommand(0x10); // Set higher Column
        
        /* Burst write data to fill the line faster*/
        i2c_start();
        i2c_write(SSD1306_ADDR << 1);
        i2c_write(0x40);
        for (uint8_t i = 0; i < 128;i++){
            i2c_write(0x00);
        }
        i2c_stop();
    }
}

/************************************************************************/
uint8_t buffer[1024];
void Setpixel(uint8_t x, uint8_t y, bool OnOff){
    if (x < 0 || x >= 128 || y < 0 || y >= 64){
        return;
    }        
        // 1. Find which "Page" (vertical band) we are in
        //    Each page is 8 pixels tall.
        //    Example: y=3 is Page 0. y=9 is Page 1.
        uint8_t page = y / 8;
        // 2. Find the bit index (0-7) inside that byte
        //    Example: y=3 is bit 3. y=9 is bit 1.
        uint8_t bit_index = y % 8;
        // 3. Calculate the array index
        //    The buffer is laid out: Page0_Col0, Page0_Col1... Page1_Col0...
        uint16_t index = (uint16_t)x +((uint16_t)page * 128);
        
        // Set or clear the bit
        if (OnOff){
            buffer[index] |= (1 << bit_index); /* On  pixel*/
        }
        else{
            buffer[index] &= ~(1 << bit_index); /* Off pixel */
        }
}
/************************************************************************/
void UpdateScreen(void){
    for (uint8_t page = 0; page < 8; page++){
        // Set the cursor to the start of the current page
        SendCommand(0xB0 + page);
        SendCommand(0x00);
        SendCommand(0x10);
        
        // Send the 128 byte for this page
        i2c_start();
        i2c_write(SSD1306_ADDR << 1);
        i2c_write(0x40);// Tell controller next bytes are DATA
        for (uint8_t x = 0; x < 128;x++){
            // Send data from our RAM buffer
            i2c_write(buffer[x + (page * 128)]);
            
        }
        i2c_stop();
    }
}
/************************************************************************/
/*uint8_t buffer[1024];*/
void ClearBuffer(void) {
    // This clears the global 'buffer' array
    memset(buffer, 0, 1024);
}                                                                     
/************************************************************************/
void drawChar(uint8_t x, uint8_t y, char c){
    if (c < 32 || c > 126){ return;}
    if (x > 122 || y > 56){return;}
        
        
        
    uint16_t offset = (c - 32)*5;
    
    for (uint8_t col = 0; col <5 ;col++){
        uint8_t line = pgm_read_byte(&font5x7[offset + col]);
        
        for (uint8_t row = 0; row <8; row++){
            if (line & (1<<row)){
                Setpixel(x + col, y + row,1);
            }
            else{
                Setpixel(x +col,y +row,0);
            }
        }
        }     
    }
/************************************************************************/
void drawCharLarge(uint8_t x, uint8_t y, char c){
    if (c < 32 || c > 126){ return;}
    if (x > 122 || y > 56){return;}
    
    
    
    uint16_t offset = (c - 32)*5;
    
    for (uint8_t col = 0; col <5 ;col++){
        uint8_t line = pgm_read_byte(&font8x8[offset + col]);
        
        for (uint8_t row = 0; row <8; row++){
            if (line & (1<<row)){
                Setpixel(x + col, y + row,1);
            }
            else{
                Setpixel(x +col,y +row,0);
            }
        }
    }
}
/************************************************************************/
