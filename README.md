# Moonshine-OS
OS+custom kernel



# how to compile and use

# you need some stuff like nasm, g++, ld and grub2-mkrescue
# and change BLOCK DEVICE VERY IMPORTANT line 18

* make -  compiles boot.asm and kernel.cpp and links to build/moonshine.bin
* make iso - does above + turns into iso with grub2-mkrescue
* make run - does above + runs with qemu
* make usb - does make iso and burns to usb with dd (change block device default is /dev/sdb)
* make clean - clears files

