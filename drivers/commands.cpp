#include "console.h"
#include "font.h"
#include <stdint.h>

void help() {
    console_speak("help in commands.cpp was calleds\n");
}

void whoami() {
    console_speak("moonshine\n");
}

void clear() {
    console_clear();
}

void version() {
    console_speak("2.1.1\n");
}