#include <stdint.h>
#include "../drivers/vga.h"

extern "C" void kernel_main(uint32_t magic, uint32_t* mbi_ptr) {
    (void)mbi_ptr;

    clear();

    if (magic != 0x36D76289) { // this has to happen im just the messenger
        set_color(12, 0);
        print("not booted by multiboot2!\n");
        return;
    }

    for (int n = 0; n < 200; n++) {
        set_color(n, (n^67) % 16);
        print("real loading screen.....\n");
    }
    
    clear();

    set_color(10, 0); // black background
    print("Moonshine-OS v0.1\n");

    set_color(15, 0);
    print("kernel loaded. peak has finally arrived\n");

set_color(12, 0);
    print("Made By Realyby And Cramangako\n");

set_color(9, 0);
    print("Check the Cmds.MD for help on commands!\n");


    while (true) { // halt forever!!!
        asm volatile("hlt");
    }
}
