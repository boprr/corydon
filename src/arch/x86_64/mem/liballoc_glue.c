#include "cpu/idt.h"
#include "mem/bitmap.h"
#include "mem/liballoc.h"
#include "mem/pmm.h"

int liballoc_lock() {
    cli();
    return 0;
}

int liballoc_unlock() {
    sti();
    return 0;
}

void* liballoc_alloc(int block) { return bitmap_alloc(phys, block); }

int liballoc_free(void* addr, int block) {
    bitmap_free(phys, addr, block);
    return 0;
}
