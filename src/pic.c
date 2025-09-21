// pic.c
#include "pic.h"
#include "io.h"

#define PIC1       0x20
#define PIC2       0xA0
#define PIC1_CMD   PIC1
#define PIC1_DATA  (PIC1+1)
#define PIC2_CMD   PIC2
#define PIC2_DATA  (PIC2+1)

#define ICW1_INIT  0x10
#define ICW1_ICW4  0x01
#define ICW4_8086  0x01

void pic_remap(void){
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PIC1_DATA, 0x20); // offset master
    outb(PIC2_DATA, 0x28); // offset slave
    outb(PIC1_DATA, 0x04); // master tem slave em IRQ2
    outb(PIC2_DATA, 0x02); // slave ligado ao IRQ2 do master
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // restaura máscaras
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic_eoi(uint8_t irq){
    if (irq >= 8) outb(PIC2_CMD, 0x20);
    outb(PIC1_CMD, 0x20);
}

void pic_set_mask(uint8_t master_mask, uint8_t slave_mask){
    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);
}
