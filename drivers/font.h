#pragma once
#include <stdint.h>
#include "../kernel/limine.h"

// Draw one character at pixel position (x, y).
// fg = foreground color, bg = background color (use make_color from kernel).
void font_draw_char(limine_framebuffer* fb, uint64_t x, uint64_t y,
                    char c, uint32_t fg, uint32_t bg);

// Draw a null-terminated string. Handles '\n' for newlines.
// Wraps to the next line automatically when the text reaches the screen edge.
void font_draw_string(limine_framebuffer* fb, uint64_t x, uint64_t y,
                      const char* str, uint32_t fg, uint32_t bg);
