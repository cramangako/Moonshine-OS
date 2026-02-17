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

    set_color(10, 0); // black background
    print("Moonshine-OS v0.1\n");

    for (int n = 0; n < 160; n++) {
        set_color(n, (n + 1) % 16);
        print("man this shit lookes ass.\n");
    }

    for (int n = 0; n < 160; n++) {
        set_color(n, (n^67) % 16);
        print("actually this looks majestic\n");
    }

    set_color(15, 0);
    clear();

    set_color(15, 0);
    print("kernel loaded. peak has finally arrived\n");

    while (true) { // halt forever!!!
        asm volatile("hlt");
    }
}
