#ifndef PMM_H
#define PMM_H

#include "boot/bootloader.h"
#include "boot/multiboot2.h"
#include "mem/bitmap.h"

extern mem_bitmap *phys;

void pmm_init();

#endif
