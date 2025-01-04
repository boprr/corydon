#include "gdt.h"

gdt_ptr gp;
gdt_entry gdt[GDT_NUM_ENTRIES];

void gdt_init() {
    gp.limit = (sizeof(gdt_entry) * GDT_NUM_ENTRIES) - 1;
    gp.base = (uint64_t)&gdt;

    gdt[0] = (gdt_entry){0};
    gdt[1] = (gdt_entry){0, 0, 0, 0x9A, 0x20, 0};
    gdt[2] = (gdt_entry){0, 0, 0, 0x92, 0, 0};
    gdt[3] = (gdt_entry){0, 0, 0, 0xFA, 0x20, 0};
    gdt[4] = (gdt_entry){0, 0, 0, 0xF2, 0, 0};

    asm volatile("lgdt %0" : : "m"(gp) : "memory");
    reload_segments();

    debugf("[\033[35;49mGDT\033[39;49m] Loaded successfully \n");
}