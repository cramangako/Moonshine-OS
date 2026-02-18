CXX      = g++
LD       = ld
LIMINE   = limine

# 64-bit, no standard library, no SSE (unsafe in kernel code)
CXXFLAGS = -std=c++17 -ffreestanding -fno-exceptions -fno-rtti -nostdlib \
           -Wall -Wextra -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
           -mcmodel=kernel -fno-pic

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

# Download and build the Limine v10 utility from source.
# In v10, the limine tool is distributed as limine.c (not a pre-built binary).
# We clone v10.x-binary from Codeberg then run make inside it to compile it.
$(LIMINE)/limine:
	rm -rf $(LIMINE)
	git clone https://codeberg.org/Limine/Limine.git $(LIMINE) \
	    --branch=v10.x-binary --depth=1
	$(MAKE) -C $(LIMINE)

# Download the Limine protocol header (limine.h) from the official
# limine-protocol repository. In v10, it is no longer bundled with
# the bootloader binaries — it lives in its own separate repo.
kernel/limine.h:
	curl -Lo $@ https://raw.githubusercontent.com/limine-bootloader/limine-protocol/HEAD/include/limine.h

build/kernel_%.o: kernel/%.cpp kernel/limine.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/drivers_%.o: drivers/%.cpp kernel/limine.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(KERNEL): $(CXX_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Build a bootable ISO image using Limine v10.
# In v10, limine.conf and the bios/uefi files go in /boot/limine/
# inside the ISO (Limine searches that path automatically).
iso: $(KERNEL) $(LIMINE)/limine
	@rm -rf iso_root
	@mkdir -p iso_root/boot/limine iso_root/EFI/BOOT
	cp $(KERNEL) iso_root/moonshine.elf
	cp limine.conf iso_root/boot/limine/
	cp $(LIMINE)/limine-bios.sys \
	   $(LIMINE)/limine-bios-cd.bin \
	   $(LIMINE)/limine-uefi-cd.bin \
	   iso_root/boot/limine/
	cp $(LIMINE)/BOOTX64.EFI iso_root/EFI/BOOT/
	cp $(LIMINE)/BOOTIA32.EFI iso_root/EFI/BOOT/
	xorriso -as mkisofs -R -r -J \
	    -b boot/limine/limine-bios-cd.bin \
	    -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
	    -apm-block-size 2048 \
	    --efi-boot boot/limine/limine-uefi-cd.bin \
	    -efi-boot-part --efi-boot-image --protective-msdos-label \
	    iso_root -o $(ISO)
	$(LIMINE)/limine bios-install $(ISO)
	@rm -rf iso_root

run: iso
	qemu-system-x86_64 -cdrom $(ISO) -m 256M

usb: iso
	sudo dd if=$(ISO) of=$(USB_DEV) bs=4M status=progress conv=fdatasync

clean:
	rm -rf build iso_root $(ISO) $(LIMINE) kernel/limine.h
