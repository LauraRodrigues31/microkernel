// keyboard.h
#pragma once
#include <stdint.h>
int  keyboard_pop_char(char* out); // 1 = OK
void keyboard_init(void);
void keyboard_isr_c(void);
