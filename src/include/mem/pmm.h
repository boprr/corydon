#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>

#include "boot/bootloader.h"
#include "boot/multiboot2.h"
#include "mem/bitmap.h"
#include "utils.h"
#include "utils/mem.h"

extern mem_bitmap *phys;

void pmm_init();

#endif
