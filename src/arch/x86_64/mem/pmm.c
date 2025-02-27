#include "mem/pmm.h"

#include <stdint.h>

#include "boot/bootloader.h"
#include "boot/multiboot2.h"
#include "mem/bitmap.h"
#include "print/printf.h"
#include "utils.h"

mem_bitmap *phys;

extern uint64_t kernel_end;

int map_size;
uintptr_t mmap_strt;

struct multiboot_tag_mmap *mmap_tag = {0};

void parse_mb2_mmap() {
    struct multiboot_tag *tag = (struct multiboot_tag *)(multiboot_data + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                       ((tag->size + 7) & ~7));
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_MMAP: {
                mmap_tag = ((struct multiboot_tag_mmap *)tag);
            }
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: {
                struct multiboot_tag_basic_meminfo *basicmem;
                basicmem = (struct multiboot_tag_basic_meminfo *)tag;
                phys->size = (basicmem->mem_lower + basicmem->mem_upper) * 1000;
                break;
            }
        }
    }

    multiboot_memory_map_t *mmap;
    int found_map_start = 0;
    uint64_t total_size = 0;
    for (mmap = ((struct multiboot_tag_mmap *)mmap_tag)->entries;
         (multiboot_uint8_t *)mmap <
         (multiboot_uint8_t *)mmap_tag + mmap_tag->size;
         mmap = (multiboot_memory_map_t *)((unsigned long)mmap +
                                           mmap_tag->entry_size)) {
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE &&
            (uint64_t)mmap->len >= phys->size / BLOCK_SIZE &&
            !found_map_start) {
            phys->map = (uint8_t *)mmap->addr;
            phys->mem_start = mmap->addr;
            found_map_start = 1;
            printf("[*] ");
        } else
            printf("    ");
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) total_size += mmap->len;
        printf(
            "base: 0x%p | length: % 17ubytes | type: %i\n",
            mmap->addr,
            mmap->len,
            mmap->type
        );
    }
    printf("total mmap size: %uGib\n", (total_size / 1073741824) + 1);
    phys->size = total_size;
}

void memset(void *_dst, int val, size_t len) {
    asm volatile("pushf; cld; rep stosb; popf"
                 :
                 : "D"(_dst), "a"(val), "c"(len)
                 : "memory");
}

void pmm_init() {
    parse_mb2_mmap();
    phys->mem_start = 0x00;

    printf(
        "[PMM]: BITMAP INFO (%p) - start: %p - size: %ubytes",
        phys->map,
        phys->mem_start,
        phys->size
    );

    memset(phys->map, 0xff, ((phys->size - 1000000) / BLOCK_SIZE));
    multiboot_memory_map_t *mmap;
    for (mmap = ((struct multiboot_tag_mmap *)mmap_tag)->entries;
         (multiboot_uint8_t *)mmap <
         (multiboot_uint8_t *)mmap_tag + mmap_tag->size;
         mmap = (multiboot_memory_map_t *)((unsigned long)mmap +
                                           mmap_tag->entry_size)) {
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
            bitmap_mark_region(
                phys, (void *)mmap->addr, mmap->len / BLOCK_SIZE, 0
            );
    }
    bitmap_mark_region(
        phys, (void *)phys->mem_start, phys->size / BLOCK_SIZE, 1
    );

    phys->used_total = 0;
    phys->used_last = phys->mem_start;
}
