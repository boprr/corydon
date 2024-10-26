#include "idt.h"

void exception_handler() {
    __asm__ volatile("cli; hlt");  // Completely hangs the computer
}

static bool vectors[MAX_IDT_ENTRIES];
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

void idt_set_entry(int index, uint64_t offset, uint16_t selector, uint8_t type_attr) {
    idt[index].isr_low = offset & 0xFFFF;
    idt[index].kernel_cs = selector;
    idt[index].ist = 0;
    idt[index].attributes = type_attr;
    idt[index].isr_mid = (offset >> 16) & 0xFFFF;
    idt[index].isr_high = (offset >> 32) & 0xFFFFFFFF;
    idt[index].reserved = 0;
}

void IDTInit() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(IDTEntry) * MAX_IDT_ENTRIES - 1;

    idt_set_entry(32, (uint64_t)exception_handler, 0x08, 0x8E);

    __asm__ volatile("lidt %0" : : "m"(idtr));  // load the new IDT
    //__asm__ volatile("sti");                    // set the interrupt flag
}
