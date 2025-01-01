#include "idt.h"

#include "dri/pic.h"
#include "gdt.h"
#include "print/debugf.h"
#include "print/printf.h"
#include "utils.h"

static const char* exceptions[] = {
    "Division Error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved",
};

static idtr idtp;
extern void* isr_stub_table[];

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
            "addr\033[34;49m \033[34;49m%p\033[39;49m \n",
            vector, isr_stub_table[vector]);
    }

    __asm__ volatile("lidt %0" : : "m"(idtp));  // load the new IDT
    __asm__ volatile("sti");                    // set the interrupt flag
}

uint8_t scancode_table[128] = {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', 'w', 'e', 'r', ' ', ' ', ' ', ' ', 'o', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', 'd', ' ', ' ', 'h', ' ', ' ', 'l',
};

void kbd_irq() {
    int scancode = inb(0x60);
    printf("%i", scancode);
}

void handle_interrupt(uint64_t stack_pointer) {
    cpu_status* status = (cpu_status*)stack_pointer;
    if (status->vector <= 31) {
        if (status->vector <= 26)
            printf("[ISR] %s \n", exceptions[status->vector]);
        panic();
    }
    if (status->vector == 33) kbd_irq();
    if (status->vector > 31) {
        pic_eoi(status->vector - 32);
    }
}