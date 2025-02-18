#include "cpu/idt.h"

#include "dri/kbd.h"
#include "dri/timer.h"
#include "print/printf.h"

static idt_ptr idtp;
__attribute__((aligned(0x10))) static idt_entry idt[256];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry* descriptor = &idt[vector];

    descriptor->isr_low = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs = GDT_KERNEL_CODE;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved = 0;
}

void idt_init() {
    idtp.base = (uintptr_t)&idt[0];
    idtp.limit = (uint16_t)sizeof(idt_entry) * 256 - 1;

    for (uint8_t vector = 0; vector <= 47; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        debugf(
            "[\033[34;49mIDT\033[39;49m] loaded \033[34;49m%i\033[39;49m at "
            "\033[34;49m0x%p\033[39;49m \n",
            vector, isr_stub_table[vector]);
    }

    idt_load();
}

void idt_load() {
    __asm__ volatile("cli");
    __asm__ volatile("lidt %0" : : "m"(idtp));
    __asm__ volatile("sti");
}

void cli() { __asm__ volatile("cli"); }

void sti() { __asm__ volatile("sti"); }

void idt_interrupt_common(uint64_t stack_pointer) {
    cpu_status* status = (cpu_status*)stack_pointer;
    if (status->vector > 31) {
        pic_eoi(status->vector - 32);
    }

    if (status->vector <= 31) {
        if (status->vector <= 26)
            panic(EXCEPTIONS[status->vector]);
        else
            panic("Unknown");
    }

    switch (status->vector) {
        case 33:
            kbd_irq();
        case 32:
            timer_irq();
    }
}
