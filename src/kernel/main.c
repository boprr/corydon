#include <stdbool.h>

#include "idt/idt.h"
#include "printf/printf.h"
#include "stdint.h"
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

void kmain() {
    printf("Booting corydon... \n");
    printf("starting Interupts...");
    IDTInit();
    printf_("  [success]");
}
