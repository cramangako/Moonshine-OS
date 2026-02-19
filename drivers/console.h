#pragma once
#include <stdint.h>
#include "../kernel/limine.h"

// set up the terminal.! call once before using console_putchar/console_print.
// fg and bg are pre-built pixel colors (use make_color in kernel.cpp).
// just check usage in kernel.cpp
void console_init(limine_framebuffer* fb, uint32_t fg, uint32_t bg);

void console_colour(limine_framebuffer* fb, uint32_t fg, uint32_t bg);

// print one character. Handles '\n' (newline), '\b' (backspace), and scrolling.
void console_putchar(char c);

// Print a null-terminated string.
void console_speak(const char* str);

void console_clear();

void check_commands();
