#ifndef VGA_H
#define VGA_H

#include <stdint.h>

// colors - use these with set_color(fg, bg)
// 0  = black
// 1  = blue
// 2  = green
// 3  = cyan
// 4  = red
// 5  = magenta
// 6  = brown
// 7  = light grey
// 8  = dark grey
// 9  = light blue
// 10 = light green
// 11 = light cyan
// 12 = light red
// 13 = light magenta
// 14 = yellow
// 15 = white

void clear();
void scroll();
void putchar(char c);
void print(const char* str);
void set_color(uint8_t fg, uint8_t bg);

#endif
