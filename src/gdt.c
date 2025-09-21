#include <stdint.h>
#include "gdt.h"

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  gran;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct gdt_entry gdt[3];
static struct gdt_ptr   gp;

extern void gdt_load(void* gdt_ptr); // em gdt_flush.asm

static void set_gdt_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[i].limit_low = (uint16_t)(limit & 0xFFFF);
    gdt[i].base_low  = (uint16_t)(base  & 0xFFFF);
    gdt[i].base_mid  = (uint8_t)((base  >> 16) & 0xFF);
    gdt[i].access    = access;
    gdt[i].gran      = (uint8_t)(((limit >> 16) & 0x0F) | (gran & 0xF0));
    gdt[i].base_high = (uint8_t)((base  >> 24) & 0xFF);
}

void gdt_init(void)
{
    // null, code(0x08), data(0x10) — 4GiB, gran=4K, 32-bit
    set_gdt_entry(0, 0,       0,       0x00, 0x00);
    set_gdt_entry(1, 0x00000, 0xFFFFF, 0x9A, 0xCF); // code
    set_gdt_entry(2, 0x00000, 0xFFFFF, 0x92, 0xCF); // data

    gp.limit = sizeof(gdt) - 1;
    gp.base  = (uint32_t)&gdt[0];

    gdt_load(&gp);
}
