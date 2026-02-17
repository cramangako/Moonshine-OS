ASM      = nasm
CXX      = g++
LD       = ld
GRUB_ISO = grub2-mkrescue

ASMFLAGS = -f elf32
CXXFLAGS = -m32 -ffreestanding -fno-exceptions -fno-rtti -nostdlib -Wall -Wextra
LDFLAGS  = -m elf_i386 -T linker.ld -nostdlib

BOOT_OBJ = build/boot.o
CXX_SRCS = $(wildcard kernel/*.cpp)
CXX_OBJS = $(patsubst kernel/%.cpp,build/%.o,$(CXX_SRCS))
KERNEL   = build/moonshine.bin
ISO      = moonshine.iso
USB_DEV  = /dev/sdb

.PHONY: all clean iso run usb

all: $(KERNEL)

$(BOOT_OBJ): boot/boot.asm
	@mkdir -p build
	$(ASM) $(ASMFLAGS) $< -o $@

build/%.o: kernel/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(KERNEL): $(BOOT_OBJ) $(CXX_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

iso: $(KERNEL)
	@cp $(KERNEL) iso/boot/moonshine.bin
	$(GRUB_ISO) -o $(ISO) iso

run: iso
	qemu-system-i386 -cdrom $(ISO)

usb: iso
	sudo dd if=$(ISO) of=$(USB_DEV) bs=4M status=progress conv=fdatasync

clean:
	rm -rf build $(ISO) iso/boot/moonshine.bin
