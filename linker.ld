ENTRY(kmain)
SECTIONS
{
  . = 0x00100000;             /* 1 MiB */

  .multiboot : { *(.multiboot) }

  .text   : { *(.text*) }
  .rodata : { *(.rodata*) }
  .data   : { *(.data*) }
  .bss    : { *(.bss*) *(COMMON) }
}
