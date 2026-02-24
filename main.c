/*
 * Testcolourreader.c
 *
 * Created: 2026-01-12 13:54:23
 * Author : alexa
 */ 

#define F_CPU 16000000UL

#include "thesauce.h"
#include "colorread.h"
#include "colorsorter.h"
#include "buttons.h"
#include "rgbfunction.h"
#include "oled.h"
#include "display_i2c.h"
#include "fotoresistor.h"
#include "lightread.h"
#include "rgb_reader_i2c.h"
#include "color_name.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

volatile uint8_t rotorpos = 0;
volatile uint8_t scan_delay = 0;
bool trueflag = false;

int main(void){
    OCR1A = 15624;
    TCCR1A = 0x00;
    TCCR1B = 0x0D;
    TIMSK1 = (1<<OCIE1A);
    PORTD |= (1<<2);
    PORTD |= (1<<3);
    EIMSK = (1<<INT0);
    EICRA = 0x01;
    sei();
    thesauce();
    
  /*  uint8_t colorvalue[4] = {0x00,0x00,0x00,0x00};*/ // ska döpas om för att göra en inscanning av temp eller nått annat för ADC omvandling 
    uint16_t rgbcvalue[4] = {0x00,0x00,0x00,0x00}; /* CRGB values are stored here as a uint16 value.*/
    uint8_t boot = 0;
    uint16_t high_low = 0;
    char display_menu[10] = "MENU";
    char display_menu_1[10] = "BOOT.";
    char display_menu_2[10] = "BOOT..";
    char display_colorscan[25] = "SCAN";
    char display_colorscan2[25] = "COLORS";
    char display_clear[6] = "";
    char display_colordelay[25] = "DELAY";
    char display_light[25] = "LIGHT";
    char display_light2[25] = "SENSOR";
    char display_colorscaner[25] = "COLOR READER";
    char display_colorscaner2[25] = "BY ALEXANDER";
    char display_version[25] = "1.0v";
    char display_process[30] = "PROCESSING.";
    char display_process2[30] = "PROCESSING..";
    char display_process_hold[20] = "HOLD STILL";
    char display_process_error[10] = "ERROR";
    char display_light_value[10] = "LUMENS";
    char display_timer[10] = "TIMER";
    char display_press[20] = "PRESS BUTTON";
    char display_start[20] = "START TIMER";
    
    
    while (1) {
      write_mode_output_portb(4,0);  
        /*boot sequence*/
  if (boot == 0){
    oleddisplay(display_menu_1,display_clear,display_clear,0);
    _delay_ms(800);
    oleddisplay(display_menu_2,display_clear,display_clear,0);
    _delay_ms(800);
    oleddisplay(display_menu_1,display_clear,display_clear,0);
    _delay_ms(800);
    oleddisplay(display_menu_2,display_clear,display_clear,0);
    _delay_ms(800);
    oleddisplay(display_clear,display_clear,display_clear,0);
    _delay_ms(500);
    oleddisplay(display_colorscaner,display_colorscaner2,display_version,0);
    boot = 1;
  }

/* Menu selector*/
    if (rotorpos == 1 && trueflag == true){
         oleddisplay(display_menu,display_clear,display_clear,0);
         trueflag = false;
    }
    /*colorread*/
   else if (rotorpos == 2 && trueflag == true){
        oleddisplay(display_colorscan,display_colorscan2,display_clear,0);
        trueflag = false;
    }
    /*Timer*/
   else if (rotorpos == 3 && trueflag == true){
        oleddisplay(display_timer,display_press,display_start,0);
        trueflag = false;
        }        
    /* function*/
   else if (rotorpos == 4 && trueflag == true){
        oleddisplay(display_colordelay,display_clear,display_clear,0);
        trueflag = false;
    }
    /*Light reader*/
   else if (rotorpos == 5 && trueflag == true){
        oleddisplay(display_light,display_light2,display_clear,0);
        trueflag = false;
    }
    
 /************************************************************************/   
    /* Colorscanning modul*/
    if (rotorpos == 2 && !buttons(0) && trueflag == false){
        oleddisplay(display_process,display_process_hold,display_clear,0);
        scan_delay = 0;
        while(scan_delay < 2){
            oleddisplay(display_process2,display_process_hold,display_clear,0);
            oleddisplay(display_process,display_process_hold,display_clear,0);
            write_mode_output_portb(4,1);
        }
        /* Start a CRGB scan and puts values in an array*/
        StartRGB(rgbcvalue);
       
        uint8_t red = (rgbcvalue[1] >>4);
        uint8_t green = (rgbcvalue[2]>>4);
        uint8_t blue = (rgbcvalue[3]>>4);
        
        high_low = rgbcvalue[0];
        
        if (high_low < 2000){ /* To read now light values*/
            red = red*3;
            green = green*3;
            blue = blue*3;
        }
        
        write_mode_output_portb(4,0);
        oleddisplay(id_color(red,green,blue),display_process_hold,display_clear,0);  
    }
/************************************************************************/

    /* Timer modul*/
    else if (rotorpos == 3 && !buttons(0) && trueflag == false){
        scan_delay = 0;
        char buff[5];
        sprintf(buff, "%u",scan_delay);
        while(buttons(0)){
        oleddisplay(buff,"Seconds",display_clear,0);
        }
        
    }
/************************************************************************/
    
    /*light modul */
    else if (rotorpos == 4 && !buttons(0) && trueflag == false ){
        StartRGB(rgbcvalue);
        uint16_t counter = rgbcvalue[0];
        char buffert_counter[6];
        sprintf(buffert_counter, "%u",counter);
        oleddisplay(buffert_counter,display_light_value,display_clear,0);
               
    }
/************************************************************************/    
    /* Temp modul*/
    else if (rotorpos == 5 && !buttons(0) && trueflag == false){
        uint16_t light_read = light();
        char buffert[4];
        sprintf(buffert, "%u",light_read);
        oleddisplay(buffert,display_clear,display_clear,0);
     
    }     
/************************************************************************/
  }
}

 
 ISR (INT0_vect){
     trueflag = true;
     if ((PIND  & (1<<2))){
         if(!(PIND & (1<<3))){
             if (rotorpos < 5){
                rotorpos ++;
             }
        }
        else{
            if (rotorpos > 0){
            rotorpos--;
            }
         }
         
     }
     
     else if (!(PIND & (1<<2)))
     {
         if(!(PIND & (1<<3))){
             if (rotorpos > 0){
                rotorpos--;
             }
             
        }
        else{
            if (rotorpos < 5){
                rotorpos++;
            }
            
         }
         
     }
 }


/*timer overflow*/
ISR (TIMER1_COMPA_vect){
    scan_delay++;
}

 
 
