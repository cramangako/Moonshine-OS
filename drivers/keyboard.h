#pragma once

// returns the ASCII character of the pressed key, or 0 if no key is ready.
// call this in a loop ,it never blocks.
char keyboard_getchar();

// slop