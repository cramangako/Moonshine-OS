#include "console.h"
#include "font.h"
#include <stdint.h>
#include "commands.h"

static limine_framebuffer* s_fb  = nullptr;
static uint32_t            s_fg  = 0;
static uint32_t            s_bg  = 0;
static uint64_t            s_cur_x = 0;
static uint64_t            s_cur_y = 0;

//defining command buffer, everything is stored (commands + arguments), clears every new line
const int buffer_size = 256;
char command_buffer[buffer_size];
static int cmd_len = 0;

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

void console_colour(limine_framebuffer* fb, uint32_t fg, uint32_t bg) {
    s_fb    = fb;
    s_fg    = fg;
    s_bg    = bg;
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
    if (c == '\n') { // newline
        s_cur_x  = 0;
        s_cur_y += CH;

    } else if (c == '\b') { // backspace
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

// only called for actual keyboard input - stores in buffer AND displays
void console_input(char c) {
    if (c == '\b') {
        if (cmd_len > 0) cmd_len--;
    } else if (c >= 32 && cmd_len < buffer_size - 1) {
        command_buffer[cmd_len++] = c;
    }
    console_putchar(c);
}

static bool streq(const char* a, const char* b) { // streq implemenation, returns true if BOTH strings are equal, else false
    while (*a && *b) {
        if (*a != *b) return false;
        a++; b++;
    }
    return *a == *b;
}

void check_commands() {
    command_buffer[cmd_len] = '\0';

    if      (streq(command_buffer, "help"))    help();
    else if (streq(command_buffer, "clear"))   clear();
    else if (streq(command_buffer, "whoami"))  whoami();
    else if (streq(command_buffer, "version")) version();
    else if (streq(command_buffer, "enjoy")) enjoy();
    else if (cmd_len > 0) {
        console_speak("unknown command: ");
        console_speak(command_buffer);
        console_speak("\n");
    }

    cmd_len = 0;
    command_buffer[0] = '\0'; // epic troll because we trick compiler into thinking buffer is wiped
}
