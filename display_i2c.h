/*
 * display_i2c.h
 *
 * Created: 2026-01-20 14:58:03
 *  Author: alexa
 */ 

#ifndef DISPLAY_I2C_H_
#define DISPLAY_I2C_H_
#include "thesauce.h"
#include "display_i2c.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
/*#include <util/delay.h>*/
#include <stdbool.h>

void i2c_write(uint8_t data);
void i2c_start(void);
void i2c_stop();
void i2c_init(void);
void SendCommand(uint8_t command);
void SendData(uint8_t data);
void Init_Screen(void);
void ClearRam(void);
void Setpixel(uint8_t x, uint8_t y, bool OnOff);
void UpdateScreen(void);
void ClearBuffer(void);
void drawChar(uint8_t x, uint8_t y, char c);
void Drawstring(uint8_t x, uint8_t y, char* text);
void drawCharLarge(uint8_t x, uint8_t y, char c);

#endif /* DISPLAY_I2C_H_ */