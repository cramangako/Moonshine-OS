#include "console.h"
#include "font.h"
#include <stdint.h>

static limine_framebuffer* s_fb  = nullptr;
static uint32_t            s_fg  = 0;
static uint32_t            s_bg  = 0;
static uint64_t            s_cur_x = 0;
static uint64_t            s_cur_y = 0;

// char cell size: 8px wide, 10px tall (8 glyph + 2 gap between lines)
static const uint64_t CW = 8;
static const uint64_t CH = 10;

void console_init(limine_framebuffer* fb, uint32_t fg, uint32_t bg) {
    s_fb    = fb;
    s_fg    = fg;
    s_bg    = bg;
    s_cur_x = 0;
    s_cur_y = 0;
}

// moves all rows up by CH pixels when the cursor hits the bottom
static void scroll() {
    uint8_t* base  = (uint8_t*)s_fb->address;
    uint64_t pitch = s_fb->pitch;
    uint64_t rows  = s_fb->height - CH;

    for (uint64_t y = 0; y < rows; y++) {
        uint32_t* dst = (uint32_t*)(base +  y      * pitch);
        uint32_t* src = (uint32_t*)(base + (y + CH) * pitch);
        for (uint64_t x = 0; x < s_fb->width; x++)
            dst[x] = src[x];
    }

    // clear the bottom strip that got left behind
    for (uint64_t y = rows; y < s_fb->height; y++) {
        uint32_t* row = (uint32_t*)(base + y * pitch);
        for (uint64_t x = 0; x < s_fb->width; x++)
            row[x] = s_bg;
    }

    s_cur_y -= CH;
}

void console_putchar(char c) {
    if (c == '\n') {
        s_cur_x  = 0;
        s_cur_y += CH;

    } else if (c == '\b') {
        if (s_cur_x >= CW) {
            s_cur_x -= CW;
        } else if (s_cur_y >= CH) {
            s_cur_y -= CH;
            s_cur_x  = (s_fb->width / CW - 1) * CW;
        }
        font_draw_char(s_fb, s_cur_x, s_cur_y, ' ', s_fg, s_bg);
        return;

    } else if (c < 32) {
        return;

    } else {
        font_draw_char(s_fb, s_cur_x, s_cur_y, c, s_fg, s_bg);
        s_cur_x += CW;

        if (s_cur_x + CW > s_fb->width) {
            s_cur_x  = 0;
            s_cur_y += CH;
        }
    }

    if (s_cur_y + CH > s_fb->height)
        scroll();
}

void console_speak(const char* str) { // not pr**t like goy linux chuds
    for (int i = 0; str[i] != '\0'; i++)
        console_putchar(str[i]);
}

void console_clear() { // floods everything with background colour
    uint8_t* base = (uint8_t*)s_fb->address;
    for (uint64_t y = 0; y < s_fb->height; y++) {
        uint32_t* row = (uint32_t*)(base + y * s_fb->pitch);
        for (uint64_t x = 0; x < s_fb->width; x++)
            row[x] = s_bg;
    }
    s_cur_x = 0;
    s_cur_y = 0;
}
