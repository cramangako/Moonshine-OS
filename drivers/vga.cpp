// vga text mode driver
// writes directly to screen memory at 0xB8000
// each cell is 2 bytes: character + color

#include "vga.h"

volatile uint16_t* screen = (volatile uint16_t*)0xB8000;

int cursor_x = 0; // current column (0-79) (top left corner of the screen)
int cursor_y = 0; // current row (0-24) (top left corner of the screen)
uint8_t color = 0x0F; // white on black

// wipe the screen and reset cursor to top left
void clear() {
    for (int i = 0; i < 80 * 25; i++) {
        screen[i] = ' ' | (color << 8);
    }
    cursor_x = 0;
    cursor_y = 0;
}

// shift every row up by one, clear the bottom row
void scroll() {
    for (int i = 0; i < 80 * 24; i++) {
        screen[i] = screen[i + 80];
    }
    for (int i = 80 * 24; i < 80 * 25; i++) {
        screen[i] = ' ' | (color << 8);
    }
    cursor_y = 24;
}

// put one character on screen, handle newlines and wrapping
void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        screen[cursor_y * 80 + cursor_x] = c | (color << 8);
        cursor_x++;
        if (cursor_x >= 80) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    if (cursor_y >= 25) {
        scroll();
    }
}

// speak a whole string by looping through each character
void speak(const char* str) {
    while (*str) {
        putchar(*str);
        str++;
    }
}

// pack foreground and background into one color byte
void set_color(uint8_t fg, uint8_t bg) {
    color = (bg << 4) | fg;
}
