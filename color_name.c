/*
 * color_name.c
 *
 * Created: 2026-02-22 10:32:18
 *  Author: alexa
 */ 
/* Color sorting module*/

#include <avr/pgmspace.h>
#include "thesauce.h"
#include "color_name.h"

typedef struct  {
    const char* name;
    uint8_t r;
    uint8_t g;
    uint8_t b;
}colorref;


const colorref PROGMEM rgb_lib[] = {
    {"Black",0x00, 0x00, 0x00},
    {"White",0xFF, 0xFF,0xFF},
    {"Red", 0xC8,0x00,0x00},
    {"Green", 0x00,0xBE,0x00},
    {"Blue", 0x00,0x00,0xC8},
    {"Yellow", 0xC8,0xC8,0x00},
    {"Magenta", 0xC8,0x00,0xC8},
    {"Cyan", 0x00,0xC8,0xC8}, 
        
};

#define sum_colors (sizeof(rgb_lib)/sizeof(colorref))


const char*  id_color(uint8_t sR,uint8_t sG, uint8_t sB){
    
    uint32_t minimumSq = 0xFFFFFFFF;
    uint8_t closest = 0;
    
    for (uint8_t i = 0; i< sum_colors; i++){
        uint8_t ref_r = pgm_read_byte(&rgb_lib[i].r); /* Plockar från PROGMEM ovanför.*/
        uint8_t ref_g = pgm_read_byte(&rgb_lib[i].g);
        uint8_t ref_b = pgm_read_byte(&rgb_lib[i].b);
        
        int32_t dr = (int32_t)sR - (int32_t)ref_r;
        int32_t dg = (int32_t)sG - (int32_t)ref_g;
        int32_t db = (int32_t)sB - (int32_t)ref_b;
        
        uint32_t currentSq = ((dr*dr)+(dg*dg)+(db*db));
        
        if (currentSq < minimumSq){
            minimumSq = currentSq;
            closest = i;
        }
        
    }
    return (const char*)pgm_read_ptr(&rgb_lib[closest].name);
    /*return rgb_lib[closest].name;*/
    
    
}
