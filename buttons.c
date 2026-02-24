/*
 * buttons.c
 *
 * Created: 2026-01-15 10:36:00
 *  Author: alexa
 */ 
#include "thesauce.h"
#include "buttons.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

uint8_t buttons(uint8_t botton){
    uint8_t on_off = 0;
    on_off= read_pullup_portb(botton);
   return on_off;       
}