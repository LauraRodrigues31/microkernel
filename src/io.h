//in/out de portas ( artigo Kernel 201)
#pragma once
#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t v; __asm__ __volatile__("inb %1,%0" : "=a"(v) : "Nd"(port));
    return v;
}
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__("outb %0,%1" :: "a"(val), "Nd"(port));
}
