# Toolchain
AS = nasm
CC = clang++
LD = ld

# Automatically find sources
KERNEL_AS_SOURCES = $(wildcard Kernel/*.asm)
KERNEL_CC_SOURCES = $(wildcard Kernel/*.cc)

# Get object files
KERNEL_OBJS := $(patsubst Kernel/%.asm,bin/Kernel/%.asm.o,$(KERNEL_AS_SOURCES)) \
               $(patsubst Kernel/%.cc,bin/Kernel/%.cc.o,$(KERNEL_CC_SOURCES))

# Flags
ASFLAGS = -f elf32 -g -F dwarf
CCFLAGS = -m32 -std=c++17 -ffreestanding -O0 -Wall -Wextra -nostdlib -I kernel -fno-stack-protector -Wno-unused-parameter -fno-stack-check -fno-lto -mno-mmx -mno-80387 -mno-sse -mno-sse2 -mno-red-zone
QEMUFLAGS = -M q35 -m 256M -cdrom bin/$(IMAGE_NAME).iso -rtc base=localtime -boot d
LDFLAGS = -m elf_i386 -TKernel/linker.ld -z noexecstack

# Output image name
IMAGE_NAME = image

all: boot Kernel iso

run: all
	@qemu-system-i386 $(QEMUFLAGS)

run-gdb: all
	@qemu-system-i386 $(QEMUFLAGS) -S -s

bin/Kernel/%.cc.o: Kernel/%.cc
	@echo " CC $<"
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CCFLAGS) -c $< -o $@

bin/Kernel/%.asm.o: Kernel/%.asm
	@echo " AS $<"
	@mkdir -p "$$(dirname $@)"
	@$(AS) $(ASFLAGS) -o $@ $<

Kernel: $(KERNEL_OBJS)
	@echo " LD Kernel/*"
	@$(LD) $(LDFLAGS) $^ -o bin/Kernel.elf

iso:
	@grub-file --is-x86-multiboot ./bin/Kernel.elf; \
	if [ $$? -eq 1 ]; then \
		echo " error: Kernel.elf is not a valid multiboot file"; \
		exit 1; \
	fi
	@mkdir -p iso_root/boot/grub/
	@cp bin/Kernel.elf iso_root/boot/Kernel.elf
	@cp boot/grub.cfg iso_root/boot/grub/grub.cfg
	@grub-mkrescue -o bin/$(IMAGE_NAME).iso iso_root/ -quiet 2>&1 >/dev/null | grep -v libburnia | cat
	@rm -rf iso_root/

clean:
	@rm -f $(BOOT_OBJS) $(KERNEL_OBJS)
	@rm -rf bin