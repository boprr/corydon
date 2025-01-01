#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#include "print/debugf.h"

#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_ptr;

void gdt_init();

#endif