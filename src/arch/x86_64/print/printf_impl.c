#include <stdint.h>

#include "print/debugf.h"
#include "print/printf.h"

static const size_t NUM_COLS = 80;
static const size_t NUM_ROWS = 25;

int line_length[25];

enum {
    PRINT_COLOR_BLACK = 0,
    PRINT_COLOR_BLUE = 1,
    PRINT_COLOR_GREEN = 2,
    PRINT_COLOR_CYAN = 3,
    PRINT_COLOR_RED = 4,
    PRINT_COLOR_MAGENTA = 5,
    PRINT_COLOR_BROWN = 6,
    PRINT_COLOR_LIGHT_GRAY = 7,
    PRINT_COLOR_DARK_GRAY = 8,
    PRINT_COLOR_LIGHT_BLUE = 9,
    PRINT_COLOR_LIGHT_GREEN = 10,
    PRINT_COLOR_LIGHT_CYAN = 11,
    PRINT_COLOR_LIGHT_RED = 12,
    PRINT_COLOR_PINK = 13,
    PRINT_COLOR_YELLOW = 14,
    PRINT_COLOR_WHITE = 15,
};

size_t colcounter = 0;
size_t rowcounter = 0;
struct Char {
    uint8_t character;
    uint8_t color;
};
const uint8_t whiteonblack = 15 | 0 << 4;

volatile struct Char* buffer = (struct Char*)(0xb8000 + 0xffffffff80000000);

void clear_screen() {
    for (int i = 0; i < (int)NUM_COLS; i++) {
        for (int j = 0; j < (int)NUM_ROWS; j++) {
            buffer[i + (NUM_COLS * j)] = (struct Char){
                character : (uint8_t)' ',
                color : whiteonblack,
            };
        }
    }
}

void move_back() {
    if (rowcounter == 0 && colcounter == 0) return;
    if (colcounter != 0) {
        colcounter--;
        printf(" ");
        colcounter--;
    } else {
        rowcounter--;
        colcounter = line_length[rowcounter];
    }
    update_cursor();

    // debugf("x:%i y:%i\n", colcounter, rowcounter);
}

void update_cursor() {
    uint16_t pos = rowcounter * NUM_COLS + colcounter;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void _putchar(char chr) {
    if (chr == '\n') {
        line_length[rowcounter] = colcounter;
        rowcounter++;
        colcounter = 0;
        return;
    }

    if (rowcounter >= NUM_ROWS) {
        rowcounter = 0;
        colcounter = 0;
        clear_screen();
    }
    buffer[colcounter + (NUM_COLS * rowcounter)] = (struct Char){
        character : (uint8_t)chr,
        color : whiteonblack,
    };
    colcounter++;
    if (colcounter >= NUM_COLS) {
        rowcounter++;
        colcounter = 0;
    }
    update_cursor();
}