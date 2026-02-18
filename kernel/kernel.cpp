#include <stdint.h>
#include "../drivers/vga.h"

extern "C" void kernel_main(uint32_t magic, uint32_t* mbi_ptr) {
    (void)mbi_ptr;

    clear();

    if (magic != 0x36D76289) { // this has to happen im just the messenger
        set_color(12, 0);
        speak("not booted by multiboot2!\n");
        return;
    }

    for (int n = 0; n < 250; n++) {
        set_color(n, (n^67) % 16);
        speak("real loading screen.....\n");
    }

    set_color(0, 0);
    clear();

    set_color(10, 0); // black background
    speak("Moonshine-OS v2.0.1\n");

    set_color(15, 0);
    speak("kernel loaded. peak has finally arrived\n");

    set_color(12, 0);
    speak("Made By Realyby And Cramangako\n");

    set_color(9, 0);
    speak("Check the Cmds.MD for help on commands!\n");

set_color(15, 0);
    speak("ⓘ This message isn't available in your region.\n");



    while (true) { // halt forever!!!
        asm volatile("hlt");
    }
}
