/*
 * oled.h
 *
 * Created: 2026-01-15 10:35:27
 *  Author: alexa
 */ 


#ifndef OLED_H_
#define OLED_H_

#include "thesauce.h"
#include "display_i2c.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void oleddisplay(char *text, char *text2,char *text3,  uint8_t tick);


#endif /* OLED_H_ */