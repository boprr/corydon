#include <stdint.h>

#include "cpu/idt.h"
#include "dri/time.h"

uint64_t timer = 0;

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
    if (timer % 1000 == 0) rtc_read_from_cmos();
}

void sleep(uint64_t ms) {
    uint64_t target = timer + ms;
    while (timer < target) __asm__ volatile("nop");
}
