// keyboard.c
#include "keyboard.h"
#include "io.h"
#include "pic.h"

#define KB_DATA   0x60
#define KB_STAT   0x64
#define BUF_SZ    64

static volatile char ring[BUF_SZ];
static volatile int head=0, tail=0;
static int shift=0;

static const char sc_to_ascii[128] = {
/*00*/0,27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
/*0F*/'\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
/*1E*/'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
/*2E*/'z','x','c','v','b','n','m',',','.','/',0,0,0,' '
};
static const char sc_to_ascii_shift[128] = {
/*00*/0,27,'!','@','#','$','%','^','&','*','(',')','_','+', '\b',
/*0F*/'\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,
/*1E*/'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
/*2E*/'Z','X','C','V','B','N','M','<','>','?',0,0,0,' '
};

static void push_char(char c){
    int nxt=(head+1)%BUF_SZ; if (nxt!=tail){ ring[head]=c; head=nxt; }
}

void keyboard_isr_c(void){
    uint8_t st = inb(KB_STAT);
    if (st & 1){
        uint8_t sc = inb(KB_DATA);

        if (sc==0x2A || sc==0x36){ shift=1; goto done; }
        if (sc==0xAA || sc==0xB6){ shift=0; goto done; }

        if (!(sc & 0x80)){
            char c = shift ? sc_to_ascii_shift[sc & 0x7F] : sc_to_ascii[sc & 0x7F];
            if (c) push_char(c);
        }
    }
done:
    pic_eoi(1); // EOI por último
}

int keyboard_pop_char(char* out){
    if (tail==head) return 0;
    *out = ring[tail];
    tail = (tail+1)%BUF_SZ;
    return 1;
}

void keyboard_init(void){
    pic_set_mask(0xFD, 0xFF); // habilita apenas IRQ1 no master
}
