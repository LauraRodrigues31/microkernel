# ====== Config ======
TARGET_DIR   := dist
ISO_DIR      := $(TARGET_DIR)/iso
KERNEL       := $(TARGET_DIR)/kernel.elf
ISO          := $(TARGET_DIR)/os.iso
LINKER_SCRIPT:= linker.ld

CC := gcc
LD := gcc
AS := nasm

CFLAGS  := -m32 -ffreestanding -fno-pic -fno-stack-protector -fno-pie -nostdlib -nostartfiles -Wall -Wextra -O2
ASFLAGS := -f elf32
LDFLAGS := -m32 -ffreestanding -nostdlib -no-pie -Wl,-T,$(LINKER_SCRIPT) -Wl,--build-id=none

CSRCS   := src/vga.c src/idt.c src/pic.c src/keyboard.c src/gdt.c src/kmain.c
COBJS   := $(CSRCS:.c=.o)
ASMOBJS := src/multiboot_header.o src/isr.o src/gdt_flush.o

# ====== Alvos ======
all: $(ISO)

run: $(ISO)
	/usr/bin/env -i HOME="$(HOME)" TERM="$(TERM)" DISPLAY="$(DISPLAY)" XAUTHORITY="$(XAUTHORITY)" \
	PATH="/usr/bin:/bin:/usr/sbin:/sbin" \
	LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu" \
	/usr/bin/qemu-system-i386 -cdrom $(ISO) -serial stdio

run-tui: $(ISO)
	/usr/bin/env -i HOME="$(HOME)" TERM="$(TERM)" \
	PATH="/usr/bin:/bin:/usr/sbin:/sbin" \
	LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu" \
	/usr/bin/qemu-system-i386 -display curses -cdrom $(ISO) -serial stdio

clean:
	rm -rf $(TARGET_DIR) src/*.o

.PHONY: all run run-tui clean

# ====== Kernel ======
$(KERNEL): $(COBJS) $(ASMOBJS) $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $(ASMOBJS) $(COBJS)

# C objects
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# ASM objects
src/%.o: src/%.asm
	$(AS) $(ASFLAGS) $< -o $@

# ====== ISO com GRUB ======
$(ISO): $(KERNEL) iso/boot/grub/grub.cfg
	@mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.elf
	cp iso/boot/grub/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR) >/dev/null 2>&1 || grub2-mkrescue -o $(ISO) $(ISO_DIR)
