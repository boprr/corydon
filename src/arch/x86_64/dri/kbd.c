#include "dri/kbd.h"

// US keyboard
static const char character_table[] = {
    0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '+',
    0,   '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '<',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '-',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   '-', 0,   0,   0,
    '+', 0,    0,    0,   0,    0,   0,   0,   '<', 0,   0,   0,
};

void kbd_irq() {
    uint8_t scancode = inb(0x60);
    if (scancode & 0x80) return;  // key up
    if (scancode == 14) {
        move_back();
        return;
    }  // backspace
    if (scancode == 28) {
        update_cursor();
        printf("\n");
        return;
    }  // Enter
    printf("%c", character_table[scancode]);
}

/*
    debugf("%i -- %i \n", scancode, shift);
    if (scancode & 0x80) return;
    if (scancode == 28) {
        printf("\n");
        update_cursor();
        return;
    }
    if (scancode == 14) {
        move_back();
        return;
    }
    if (scancode > 139) return;
    if (shift == 1)
        printf("%c", character_table_shift[scancode]);
    else
        printf("%c", character_table[scancode]);
*/
