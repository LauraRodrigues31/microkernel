// vga.c
#include "vga.h"

#define VGA_MEM ((uint16_t*)0xB8000)
static uint8_t row=0, col=0;
static uint8_t color=0x0F;

static void put_at(char c, uint8_t r, uint8_t c2){
    VGA_MEM[r*80 + c2] = ((uint16_t)color << 8) | (uint8_t)c;
}

void vga_init(void){ vga_clear(); }

void vga_clear(void){
    for (int i=0;i<80*25;i++) VGA_MEM[i] = ((uint16_t)color<<8) | ' ';
    row=col=0;
}

void vga_putc(char c){
    if (c=='\n'){ col=0; if(++row==25) row=24; return; }
    put_at(c,row,col);
    if(++col==80){ col=0; if(++row==25) row=24; }
}

void vga_write(const char* s){ while(*s) vga_putc(*s++); }
