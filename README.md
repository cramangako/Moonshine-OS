# Moonshine-OS 
a operating system with: - Custom kernel -
Bootloader setup using GRUB - Build system with Makefile

# Version: Prototype 2.0.1

  ----------------------------------------
  Requirements (What you need installed)
  ----------------------------------------

Before building Moonshine-OS, install these tools:

-   nasm → assembles the boot code
-   g++ → compiles the kernel (C++)
-   ld → links everything into a binary
-   grub2-mkrescue → creates a bootable ISO
-   qemu (optional) → runs the OS in a virtual machine

On Debian/Ubuntu you can install most of these with:

sudo apt install build-essential nasm grub-pc-bin xorriso qemu-system

  -------------------------------
  IMPORTANT WARNING (USB users)
  -------------------------------

If you plan to use make usb, you MUST edit the block device in the
Makefile.

Default is:

/dev/sdb

If this is wrong, you could accidentally wipe your main drive.

Check your USB device with:

lsblk

Then update the correct device in the Makefile (line 18).

  -----------------------------------------
  Build Commands (What each command does)
  -----------------------------------------

Compile the OS:

make

This will: 1. Assemble boot.asm 2. Compile kernel.cpp 3. Link them into
build/moonshine.bin

------------------------------------------------------------------------

Create a Bootable ISO:

make iso

Does everything from make plus creates:

build/moonshine.iso

You can burn this to a CD or use it in a VM.

------------------------------------------------------------------------

Run in QEMU (Recommended for beginners):

make run

This: 1. Builds the OS 2. Creates the ISO 3. Launches it in QEMU
automatically

No USB needed.

------------------------------------------------------------------------

Write to a USB Drive:

make usb

This: 1. Builds the ISO 2. Uses dd to flash it to your USB drive

Again — double check the block device first.

------------------------------------------------------------------------

Clean Build Files:

make clean

Removes generated files so you can rebuild from scratch.

------------------------------------------------------------------------

Beginner Advice

If you’re new to OS development, start with:

make run

It’s the safest way to test without risking your computer.