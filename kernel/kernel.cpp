#include <stdint.h>
#include "limine.h"
#include "../drivers/vga.h"

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(2);

// ask limine for the hhdm offset so we can access physical memory
__attribute__((used, section(".limine_requests")))
static volatile limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

extern "C" void kernel_main() {
    // must happen first — tells vga driver where physical memory is mapped
    uint64_t hhdm = hhdm_request.response ? hhdm_request.response->offset : 0;
    vga_init(hhdm);

    clear();

    for (int n = 0; n < 250; n++) {
        set_color(n, (n^67) % 16);
        speak("real loading screen.....\n");
    }

    set_color(0, 0);
    clear();

    set_color(10, 0);
    speak("Moonshine-OS v2.0.1\n");

    set_color(15, 0);
    speak("kernel loaded. peak has finally arrived\n");

    set_color(12, 0);
    speak("Made By Realyby And Cramangako\n");

    set_color(9, 0);
    speak("Check the Cmds.MD for help on commands!\n");

    set_color(15, 0);
    speak("This message isn't available in your region.\n");

    while (true) { // halt forever!!!
        asm volatile("hlt");
    }
}
