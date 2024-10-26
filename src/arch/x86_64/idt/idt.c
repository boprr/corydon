#include "idt.h"

#include <stdint.h>

#include "printf/printf.h"

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}
static inline void outportb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

void exception_handler() {
    //__asm__ volatile("cli; hlt");  // Completely hangs the computer
}
extern void* isr_stub_table[];

static IDTPointer idtr;

void IDTSetDescriptor(uint8_t vector, void* isr, uint8_t flags) {
    IDTEntry* descriptor = &idt[vector];

    descriptor->isr_low = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs = GDT_OFFSET_KERNEL_CODE;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved = 0;
}

extern void kbrd_intr();
extern void NULLDISC();

void IDTInit() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(IDTEntry) * MAX_IDT_ENTRIES - 1;

    for (int i = 0; i < 31; i++) {
        IDTSetDescriptor(i, (uint64_t)isr_stub_table[i], 0x8E);
    }
    // IDTSetDescriptor(33, (uint64_t)keybrd, 0x8E);
    // IDTSetDescriptor(33, (uint64_t)kbrd_intr, 0x8E);
    // KILL MEEE

    asm volatile("lidt %0" ::"m"(idtr) : "memory");
    asm volatile("sti");
}
