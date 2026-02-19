#include "keyboard.h"
#include <stdint.h>

// read a byte from an x86 I/O port
// 0x60 = keyboard data, 0x64 = keyboard status
static uint8_t inb(uint16_t port) {
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// scancode set 1, no shift held
// index is the scancode, value is the ASCII char (0 = skip)
static const char scancode_ascii[58] = {
    0,     // 0x00
    0,     // 0x01 Escape
    '1','2','3','4','5','6','7','8','9','0','-','=',  // 0x02-0x0D
    '\b',  // 0x0E Backspace
    '\t',  // 0x0F Tab
    'q','w','e','r','t','y','u','i','o','p',          // 0x10-0x19
    '[',']',                                           // 0x1A-0x1B
    '\n',  // 0x1C Enter
    0,     // 0x1D Left Ctrl
    'a','s','d','f','g','h','j','k','l',              // 0x1E-0x26
    ';','\'','`',                                      // 0x27-0x29
    0,     // 0x2A Left Shift
    '\\',  // 0x2B
    'z','x','c','v','b','n','m',                       // 0x2C-0x32
    ',','.','/',                                       // 0x33-0x35
    0,     // 0x36 Right Shift
    '*',   // 0x37 Numpad *
    0,     // 0x38 Alt
    ' ',   // 0x39 Space
};

// same layout but with shift held
static const char scancode_ascii_shift[58] = {
    0,
    0,
    '!','@','#','$','%','^','&','*','(',')','_','+',
    '\b',
    '\t',
    'Q','W','E','R','T','Y','U','I','O','P',
    '{','}',
    '\n',
    0,
    'A','S','D','F','G','H','J','K','L',
    ':','"','~',
    0,
    '|',
    'Z','X','C','V','B','N','M',
    '<','>','?',
    0,
    '*',
    0,
    ' ',
};

static bool shift_held = false;

char keyboard_getchar() {
    // bit 0 of 0x64 goes high when the keyboard has a byte waiting
    if (!(inb(0x64) & 0x01)) return 0;

    uint8_t scancode = inb(0x60);

    // key release scancodes have bit 7 set
    if (scancode & 0x80) {
        uint8_t key = scancode & 0x7F;
        if (key == 0x2A || key == 0x36) shift_held = false;
        return 0;
    }

    if (scancode == 0x2A || scancode == 0x36) {
        shift_held = true;
        return 0;
    }

    if (scancode >= sizeof(scancode_ascii)) return 0;

    return shift_held ? scancode_ascii_shift[scancode] : scancode_ascii[scancode];
}
