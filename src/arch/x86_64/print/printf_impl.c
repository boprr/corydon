#include <stdint.h>

#include "print/printf.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

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
    for (int i = 0; i < NUM_COLS; i++) {
        for (int j = 0; j < NUM_ROWS; j++) {
            buffer[i + (NUM_COLS * j)] = (struct Char){
                character : (uint8_t)' ',
                color : whiteonblack,
            };
        }
    }
}

void _putchar(char chr) {
    if (chr == '\n') {
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
}