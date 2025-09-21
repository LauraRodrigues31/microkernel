;setup do teclado + lidt
BITS 32
global isr_keyboard_stub
global idt_flush

extern keyboard_isr_c

; Stub IRQ1 (vetor 0x21 depois do remapeamento)
isr_keyboard_stub:
    pusha
    call keyboard_isr_c
    popa
    iretd

; lidt(idt_ptr)
idt_flush:
    mov eax, [esp+4]
    lidt [eax]
    ret
