#include "kernel.h"

#define ANSI_GREEN_BG_WHITE_FG "\x1b[37;42m"
#define ANSI_CLEAR_HOME "\x1b[2J\x1b[H"

void console_init(void) {
    vga_init();
    serial_init();
    keyboard_init();

    /* In -nographic mode, serial is rendered by the host terminal. */
    serial_write(ANSI_GREEN_BG_WHITE_FG);
    serial_write(ANSI_CLEAR_HOME);
}

void console_putc(char c) {
    vga_putc(c);
    serial_putc(c);
}

void console_write(const char *s) {
    vga_write(s);
    serial_write(s);
}

void console_writeln(const char *s) {
    console_write(s);
    console_putc('\n');
}

void console_clear(void) {
    vga_clear();
    serial_write(ANSI_CLEAR_HOME);
}

char console_getc_blocking(void) {
    for (;;) {
        if (serial_received()) {
            return serial_read();
        }

        if (keyboard_has_char()) {
            char c = keyboard_get_char();
            if (c) {
                return c;
            }
        }
    }
}
