#ifndef PMM_H
#define PMM_H

#include "mem/bitmap.h"
#include "boot/multiboot2.h"
#include "boot/bootloader.h"

extern mem_bitmap *phys;

void pmm_init(); 

#endif
