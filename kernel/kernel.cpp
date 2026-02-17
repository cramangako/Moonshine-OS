#include <stdint.h>

// The screen lives at this memory address
volatile uint16_t* screen = (volatile uint16_t*)0xB8000;

// Write a string to the screen
// Each cell = character | (color << 8)
// 0x0F = white on black
void print(const char* str) {
    int i = 0;
    while (*str) {
        screen[i] = *str | (0x0F << 8);
        str++;
        i++;
    }
}

// Entry point — called from boot.asm
extern "C" void kernel_main(uint32_t magic, uint32_t* mbi_ptr) {
    (void)magic;
    (void)mbi_ptr;

    print("Hello from Moonshine-OS!");

    while (true) {
        asm volatile("hlt");
    }
}
