#include "dri/timer.h"

#include <stdint.h>

#include "cpu/idt.h"
#include "print/printf.h"

uint64_t timer;

void timer_init() {
    uint32_t divisor = PIT_FREQ / TIMER_HZ;

    outb(0x43, 0b00110100);

    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
    timer = 0;
}

void timer_irq() {
    cli();
    timer++;
}
