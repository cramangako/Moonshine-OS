# Moonshine-OS
a operating system with: - Custom C++ kernel -
Bootloader setup using Limine - Build system with Makefile

# Version: Prototype 2.1.0

  ----------------------------------------
  Requirements (What you need installed)
  ----------------------------------------

Before building Moonshine-OS, install these tools:

-   g++ → compiles the kernel (C++)
-   ld → links everything into a binary
-   xorriso → creates a bootable ISO
-   curl → downloads limine.h on first build
-   qemu (optional) → runs the OS in a virtual machine

On Debian/Ubuntu you can install most of these with:

sudo apt install build-essential xorriso curl qemu-system

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

This will: 1. Download limine.h if missing 2. Compile kernel.cpp and all
drivers 3. Link them into build/moonshine.elf

------------------------------------------------------------------------

Create a Bootable ISO:

make iso

Does everything from make plus creates:

moonshine.iso

You can burn this to a USB or use it in a VM.

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

Made with love by Realyby And Cramangako
💙
