#ifndef IDT_H
#define IDT_H
#include <stdint.h>

#include "cpu/gdt.h"
#include "dri/kbd.h"
#include "dri/pic.h"
#include "dri/time.h"
#include "print/printf.h"
#include "utils.h"

typedef struct {
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__((packed)) idt_entry;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_ptr;

extern const char* EXCEPTIONS[];
extern void* isr_stub_table[];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init(void);
void idt_load(void);
void idt_interrupt_common(uint64_t stack_pointer);
void cli();
void sti();

#endif
