BITS 32
global gdt_load

; void gdt_load(void* gdt_ptr);
gdt_load:
    mov eax, [esp+4]
    lgdt [eax]

    mov ax, 0x10        ; data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush     ; far jump para carregar CS=0x08
.flush:
    ret
