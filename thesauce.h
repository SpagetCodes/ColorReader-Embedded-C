/*
 * thesauce.h
 *
 * Created: 2026-01-12 15:15:13
 *  Author: alexa
 */ 

/*#define F_CPU 16000000UL*/
#ifndef THESAUCE_H_
#define THESAUCE_H_
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

void thesauce(void);

bool read_mode_input_portd(uint8_t position);
bool read_mode_input_portc(uint8_t position);
bool read_mode_input_portb(uint8_t position);

void write_mode_output_portb(uint8_t position, bool value);
void write_mode_output_portc(uint8_t position, bool value);
void write_mode_output_portd(uint8_t position, bool value);

bool read_pullup_portb(uint8_t position);
uint16_t analog_input_to_digital(uint8_t position);

void interupt_active(void);
void interrupt_timer(void);

#endif /* THESAUCE_H_ */