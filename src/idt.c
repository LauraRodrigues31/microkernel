// idt.c
#include "idt.h"

extern void idt_flush(uint32_t);
extern void isr_keyboard_stub(void);

static struct idt_entry idt[256];
static struct idt_ptr   idtp;

static void idt_set_gate(int n, uint32_t handler){
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = 0x08;   // code selector da nossa GDT
    idt[n].zero        = 0;
    idt[n].type_attr   = 0x8E;   // present, ring0, 32-bit interrupt gate
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void idt_init(void){
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt[0];

    for (int i=0;i<256;i++){
        idt[i].offset_low=0; idt[i].selector=0; idt[i].zero=0;
        idt[i].type_attr=0; idt[i].offset_high=0;
    }

    // IRQ1 teclado (PIC remapeado: 0x20 + 1)
    idt_set_gate(0x21, (uint32_t)isr_keyboard_stub);

    idt_flush((uint32_t)&idtp);
}
