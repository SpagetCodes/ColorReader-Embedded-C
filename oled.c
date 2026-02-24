/*
 * oled.c
 *
 * Created: 2026-01-15 10:33:44
 *  Author: alexa
 */ 
#define F_CPU 16000000UL 
#include "thesauce.h"
#include "display_i2c.h"
#include "oled.h"
#include "font.h"
#include "buttons.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#define SSD1306_ADDR 0x3C
uint8_t buffer[1024];

void oleddisplay(char *text, char *text2,char *text3, uint8_t tick){
    i2c_init();
    Init_Screen();

// 1. Clear RAM buffer
    ClearBuffer();

// 2. Rita text rad 1
uint8_t i = 0;
while (text[i] != '\0' && (i * 10) < 128) {  /* kollar slutet på texten och att det endast kan vara 128 pixlar långt.*/
    drawChar(10 * i, 10, text[i]);  /* 10* i är bundet till ACCI Arryn som vi skapat i main och där är varje bokstav 10 pixlar bred.*/ 
    i++; /* för att hoppa ur loopen*/
}

// 3. Rita text rad 2
i = 0;
while (text2[i] != '\0' && (i * 10) < 128) {
    drawChar(10 * i, 30, text2[i]); 
    i++;
}

// 4. Rita text rad 3
i = 0;
while (text3[i] != '\0' && (i * 10) < 128) {
    drawChar(10 * i, 50, text3[i]); /* 50 är vilken höjd på y axel som texten ligger.*/
    i++;
}

// 4. Send buffer to screen
UpdateScreen();
 _delay_ms(1);
return;

}




/* TEST CODE FOR DISPLAY*/
// 2. FORCE TEST: Manually draw a box in the buffer (10x10 pixels)
// for(uint8_t i = 0; i < 10; i++) {
//     for(uint8_t j = 0; j < 10; j++) {
//         Setpixel(i, j, 1);
//     }
// }

// 3. Draw a character
// for (uint8_t i = 0; i < 9; i++){  /* Öka i för längre text*/
//
//         drawChar(10*i, 10, *(text+i) );
//         drawChar(10*i, 30, *(text2+i) );
//         drawChar(10*i, 50, *(text3+i) );
//
//
//
//
// }
