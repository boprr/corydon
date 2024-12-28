#include "idt.h"

#include <stdint.h>

#include "print/debugf.h"
#include "print/printf.h"
#include "utils.h"

static idtr idtp;
extern void* isr_stub_table[];

__attribute__((aligned(0x10))) static idt_entry idt[256];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry* descriptor = &idt[vector];

    descriptor->isr_low = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved = 0;
}

void remap_pic() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);
}

void idt_init() {
    remap_pic();
    idtp.base = (uintptr_t)&idt[0];
    idtp.limit = (uint16_t)sizeof(idt_entry) * 256 - 1;

    for (uint8_t vector = 0; vector <= 47; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        debugf(
            "[\033[34;49mIDT\033[39;49m] loaded \033[34;49m%i\033[39;49m at "
            "addr\033[34;49m \033[34;49m%p\033[39;49m \n",
            vector, isr_stub_table[vector]);
    }

    __asm__ volatile("lidt %0" : : "m"(idtp));  // load the new IDT
    __asm__ volatile("sti");                    // set the interrupt flag
}

void handle_interrupt(uint64_t stack_pointer) {
    cpu_status* status = (cpu_status*)stack_pointer;
    if (status->vector <= 31) {
        if (status->vector <= 26)
            printf("[ISR] %s \n", exceptions[status->vector]);
        panic(status);
    }
    if (status->vector >= 40) {
        outb(0xA0, 0x20);
    }
    if (status->vector >= 32 && status->vector <= 47) {
        outb(0x20, 0x20);
    }
}