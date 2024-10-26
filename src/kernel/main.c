#include <stdbool.h>

#include "idt/idt.h"
#include "printf/printf.h"
#include "stdint.h"

static inline uint8_t ainb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

void kmain() {
    IDTInit();
    printf("idt is working!");
}