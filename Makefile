ASM      = nasm
CXX      = g++
LD       = ld
GRUB_ISO = grub2-mkrescue

ASMFLAGS = -f elf32
CXXFLAGS = -m32 -ffreestanding -fno-exceptions -fno-rtti -nostdlib -Wall -Wextra
LDFLAGS  = -m elf_i386 -T linker.ld -nostdlib

BOOT_OBJ = build/boot.o
KERN_SRCS = $(wildcard kernel/*.cpp)
DRV_SRCS  = $(wildcard drivers/*.cpp)
KERN_OBJS = $(patsubst kernel/%.cpp,build/kernel_%.o,$(KERN_SRCS))
DRV_OBJS  = $(patsubst drivers/%.cpp,build/drivers_%.o,$(DRV_SRCS))
CXX_OBJS  = $(KERN_OBJS) $(DRV_OBJS)
KERNEL   = build/moonshine.bin
ISO      = moonshine.iso
USB_DEV  = /dev/sdb

.PHONY: all clean iso run usb

all: $(KERNEL)

$(BOOT_OBJ): boot/boot.asm
	@mkdir -p build
	$(ASM) $(ASMFLAGS) $< -o $@

build/kernel_%.o: kernel/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/drivers_%.o: drivers/%.cpp
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
