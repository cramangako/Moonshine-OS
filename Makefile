CXX      = g++
LD       = ld
LIMINE   = limine

# 64-bit, no standard library, no SSE (unsafe in kernel code)
CXXFLAGS = -std=c++17 -ffreestanding -fno-exceptions -fno-rtti -nostdlib \
           -Wall -Wextra -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
           -mcmodel=kernel

LDFLAGS  = -T linker.ld -nostdlib -zmax-page-size=0x1000

KERN_SRCS = $(wildcard kernel/*.cpp)
DRV_SRCS  = $(wildcard drivers/*.cpp)
KERN_OBJS = $(patsubst kernel/%.cpp,build/kernel_%.o,$(KERN_SRCS))
DRV_OBJS  = $(patsubst drivers/%.cpp,build/drivers_%.o,$(DRV_SRCS))
CXX_OBJS  = $(KERN_OBJS) $(DRV_OBJS)

KERNEL   = build/moonshine.elf
ISO      = moonshine.iso
USB_DEV  = /dev/sdb

.PHONY: all clean iso run usb

all: $(KERNEL)

# download limine bootloader if not already present
$(LIMINE):
	git clone https://github.com/limine-bootloader/limine.git \
	    --branch=v8.x-binary --depth=1 $(LIMINE)
	$(MAKE) -C $(LIMINE)

# download limine.h if not already present
kernel/limine.h:
	curl -Lo $@ https://raw.githubusercontent.com/limine-bootloader/limine/v8.x-binary/limine.h

build/kernel_%.o: kernel/%.cpp kernel/limine.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/drivers_%.o: drivers/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(KERNEL): $(CXX_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

iso: $(KERNEL) $(LIMINE)
	@mkdir -p iso_root/EFI/BOOT
	cp $(KERNEL) iso_root/moonshine.elf
	cp limine.conf iso_root/
	cp $(LIMINE)/limine-bios.sys $(LIMINE)/limine-bios-cd.bin \
	   $(LIMINE)/limine-uefi-cd.bin iso_root/
	cp $(LIMINE)/BOOTX64.EFI iso_root/EFI/BOOT/
	xorriso -as mkisofs \
	    -b limine-bios-cd.bin \
	    --no-emul-boot --boot-load-size 4 --boot-info-table \
	    --efi-boot limine-uefi-cd.bin \
	    -efi-boot-part --efi-boot-image --protective-msdos-label \
	    iso_root -o $(ISO)
	$(LIMINE)/limine bios-install $(ISO)
	@rm -rf iso_root

run: iso
	qemu-system-x86_64 -cdrom $(ISO) -m 256M

usb: iso
	sudo dd if=$(ISO) of=$(USB_DEV) bs=4M status=progress conv=fdatasync

clean:
	rm -rf build iso_root $(ISO)
