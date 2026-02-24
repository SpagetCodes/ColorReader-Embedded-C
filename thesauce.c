/*
 * thesauce.c
 *
 * Created: 2026-01-12 14:01:04
 *  Author: alexa
 */ 
#define F_CPU 16000000UL
#include "thesauce.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <math.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <time.h>

void thesauce(void){
    
    /* if its a 1 it will act as a output and if its a 0 it will act as a input.*/
    /*PINS_D choose input or output.*/
        DDRD = 0b11110011; /* All are input if 0x00 or 0b00000000*/
    /*PINS_B choose input or output.*/ 
        DDRB = 0b00110000; /* All are out put if 0xFF*/
    /*PINS_C choose input or output. These also have AD To DC*/
        DDRC = 0b00000000; /* All are input if 0x00*/
}
   
   
    /*Choose which pins you want to read*/     
    /* Read input portB*/
    bool read_mode_input_portb(uint8_t position) {
        return PINB & (1 << position);
    }
       
    /* Read input portC*/
    bool read_mode_input_portc(uint8_t position){
        return PINC & (1 << position);
    }
    /* Read input portD*/
    bool read_mode_input_portd(uint8_t position){
        return PIND & (1 << position);
    }
   
    
    /* Pull up resistor read mode*/
    
   /* PortB read pullup*/
     bool read_pullup_portb(uint8_t position){
         PORTB |= (1 << position);
         return PINB &= (1 << position);
     } 
        
    /* Choose what pins you would like to write*/
    
    /* Put PORTB in Output mode*/
    void write_mode_output_portb(uint8_t position, bool value){
        if (value){ /* if value is 0 it will sett port at 0 even if it was set as output in the DDRB*/
            PORTB |= (1 << position);
        }
        else{
            PORTB &= ~(1 << position); 
        }
    }    
     void write_mode_output_portd(uint8_t position, bool value){
         if (value){ /* if value is 0 it will sett port at 0 even if it was set as output in the DDRD*/
             PORTD |= (1 << position);
         }
         else{
             PORTD &= ~(1 << position);
         }
     }    
    void write_mode_output_portc(uint8_t position, bool value){
        if (value){ /* if value is 0 it will sett port at 0 even if it was set as output in the DDRC*/
            PORTC |= (1 << position);
        }
        else{
            PORTC &= ~(1 << position);
        }
    }
    
    /* Analog to Digtial gives a number between 0-1024 resolution 10bit */
    
    uint16_t analog_input_to_digital(uint8_t position){
        ADMUX = ((1 << REFS0) | position);
        ADCSRA = ((1 << ADEN) | (1 << ADSC) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2));
        while ((ADCSRA & (1 << ADIF)) == 0) ;
        ADCSRA = (1 << ADIF);
        return ADC;
    }
    
    /*Interrupts for PIND2 and PIND3 */
    void interupt_active(void){
        PORTD = 1<<2; /* Activates Pull up on port 2D*/
        PORTD = 1<<3;
        EIMSK = (1<<INT0); /* Activates external interrupts 0*/
        /*EIMSK = (1<<INT1);*/
        EICRA |= (1<<ISC01);
        sei (); /* enables interrupts*/
       
    }
    void interrupt_timer(void){
        OCR1A = 15624;
        TCCR1A = 0x00;
        TCCR1B = 0x0D;
        TIMSK1 = (1<<OCIE1A);
        sei();
    }
   
   



