# Moonshine-OS
OS+custom kernel

wip

# how to compile and use

* make -  compiles boot.asm and kernel.cpp and links to build/moonshine.bin
* make iso - does above + turns into iso with grub2-mkrescue
* make run - does above + runs with qemu
* make usb - does make iso and burns to usb with dd (change block device default is /dev/sdb)
* make clean - clears files

