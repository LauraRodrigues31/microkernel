# Microkernel – Jogo da Velha (GRUB/Multiboot, GDT/IDT, IRQ1)
* Ponderada do Nicola 

## Kernel x86 32-bit minimalista que:
- boota via **GRUB/Multiboot**;
- instala **GDT** própria (CS=0x08, DS=0x10);
- remapeia **PIC** (0x20..0x2F) e configura **IDT** (vetor 0x21 para IRQ1 teclado);
- lê teclado via portas **0x60/0x64** e imprime em VGA (0xB8000);
- roda um **jogo da velha** interativo (W/A/S/D, Espaço/Enter, R, ESC).

* Como referência utilizei bastante os artigos Kernels 101 e 201 

## Requisitos
```bash
sudo apt update
sudo apt install -y build-essential nasm qemu-system-x86 grub-pc-bin xorriso mtools gcc-multilib
```
## Como rodar 
```bash
make
make run
```
## controles do jogo 
* W/A/S/D move o cursor 
* espaço ou enter: marcar
R: reiniciar 
ESC: sair