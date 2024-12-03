#include "panic.h"

void panic() {
    clear_screen();
    printf("[Critical] panic! \n");
    asm volatile("cli");
    asm volatile("hlt");
}