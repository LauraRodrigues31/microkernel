#include <stdint.h>
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"

static inline void sti(void){ __asm__ __volatile__("sti"); }
static inline void hlt(void){ __asm__ __volatile__("hlt"); }

typedef enum { PLAYING=0, WIN, DRAW } GameState;

static char board[3][3];
static int cur_r=1, cur_c=1;
static char current='X';
static GameState state=PLAYING;

static void reset_game(void){
    for(int r=0;r<3;r++) for(int c=0;c<3;c++) board[r][c]=' ';
    cur_r=cur_c=1; current='X'; state=PLAYING;
}

static void header(void){
    vga_write("MICROKERNEL (GRUB/Multiboot) — Jogo da Velha\n");
    vga_write("W/A/S/D move | Enter/Espaco marca | R reinicia | ESC sai\n\n");
}

static void draw_board(void){
    vga_clear(); header();
    for(int r=0;r<3;r++){
        vga_write("   ");
        for(int c=0;c<3;c++){
            if(r==cur_r && c==cur_c) vga_putc('['); else vga_putc(' ');
            vga_putc(board[r][c]==' ' ? ' ' : board[r][c]);
            if(r==cur_r && c==cur_c) vga_putc(']'); else vga_putc(' ');
            if(c<2) vga_write(" | ");
        }
        vga_putc('\n');
        if(r<2) vga_write("   ---+-----+---\n");
    }
    vga_putc('\n');
}

static int line_win(char a, char b, char c){ return (a!=' ' && a==b && b==c); }

static GameState evaluate(char* w){
    for(int r=0;r<3;r++) if(line_win(board[r][0],board[r][1],board[r][2])){*w=board[r][0];return WIN;}
    for(int c=0;c<3;c++) if(line_win(board[0][c],board[1][c],board[2][c])){*w=board[0][c];return WIN;}
    if(line_win(board[0][0],board[1][1],board[2][2])){*w=board[0][0];return WIN;}
    if(line_win(board[0][2],board[1][1],board[2][0])){*w=board[0][2];return WIN;}
    int filled=0; for(int r=0;r<3;r++) for(int c=0;c<3;c++) if(board[r][c]!=' ') filled++;
    return (filled==9)?DRAW:PLAYING;
}

static void footer(void){
    if(state==PLAYING){ vga_write("Vez de: "); vga_putc(current); vga_putc('\n'); }
    else if(state==WIN){ char w=' '; (void)evaluate(&w); vga_write("Vencedor: "); vga_putc(w); vga_write(" | R reinicia\n"); }
    else vga_write("Empate! R reinicia\n");
}

static inline void handle_char(char ch){
    if (ch==27){ vga_write("\nEncerrando...\n"); for(;;) hlt(); }
    if (state!=PLAYING){ if(ch=='r'||ch=='R'){ reset_game(); draw_board(); footer(); } return; }

    switch(ch){
        case 'w': case 'W': if(cur_r>0) cur_r--; break;
        case 's': case 'S': if(cur_r<2) cur_r++; break;
        case 'a': case 'A': if(cur_c>0) cur_c--; break;
        case 'd': case 'D': if(cur_c<2) cur_c++; break;
        case ' ': case '\n':
            if(board[cur_r][cur_c]==' '){
                board[cur_r][cur_c]=current;
                char w=' '; state=evaluate(&w);
                if(state==PLAYING) current=(current=='X')?'O':'X';
            }
            break;
        default: break;
    }
    draw_board(); footer();
}

void kmain(void){
    vga_init();
    vga_write("Inicializando GDT/IDT/PIC/Teclado...\n");

    gdt_init();      // garante CS=0x08 e DS..=0x10
    pic_remap();     // remapeia 0x20..0x2F
    idt_init();      // define handler 0x21
    keyboard_init(); // habilita IRQ1
    sti();           // ativa interrupções

    reset_game();
    draw_board(); footer();

    while(1){
        char c;
        if (keyboard_pop_char(&c)) handle_char(c);
        hlt();
    }
}
