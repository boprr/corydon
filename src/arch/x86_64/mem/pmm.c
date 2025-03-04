#include "mem/pmm.h"

mem_bitmap *phys;
struct multiboot_tag_mmap *mmap_tag = {0};

void parse_mb2_mmap() {
    struct multiboot_tag *tag = (struct multiboot_tag *)(multiboot_data + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_MMAP: {
                mmap_tag = ((struct multiboot_tag_mmap *)tag);
                break;
            }
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: {
                struct multiboot_tag_basic_meminfo *basicmem;
                basicmem = (struct multiboot_tag_basic_meminfo *)tag;
                phys->size = (basicmem->mem_lower + basicmem->mem_upper) * 1000;
                break;
            }
        }

        tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                       ((tag->size + 7) & ~7));
    }

    int found_map_start = 0;
    int found_mem_start = 0;
    uint64_t mem_avil = 0;

    multiboot_memory_map_t *mmap =
        ((struct multiboot_tag_mmap *)mmap_tag)->entries;
    uint8_t *mmap_end = (uint8_t *)mmap_tag + mmap_tag->size;

    while ((uint8_t *)mmap < mmap_end) {
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE && !found_mem_start) {
            found_mem_start = 1;
            phys->mem_start = mmap->addr;
        }
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE &&
            mmap->len >= (phys->size / BLOCK_SIZE) && !found_map_start) {
            found_map_start = 1;
            phys->map = (uint8_t *)mmap->addr;
        }
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) mem_avil += mmap->len;

        mmap =
            (multiboot_memory_map_t *)((uint64_t)mmap + mmap_tag->entry_size);
    }

    if (!found_map_start) panic("Not enough memory");

    phys->size = mem_avil;
}

void pmm_init() {
    parse_mb2_mmap();

    memset(phys->map, 0xff, (phys->size / BLOCK_SIZE));

    multiboot_memory_map_t *mmap =
        ((struct multiboot_tag_mmap *)mmap_tag)->entries;
    uint8_t *mmap_end = (uint8_t *)mmap_tag + mmap_tag->size;

    while ((uint8_t *)mmap < mmap_end) {
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
            bitmap_mark_region(
                phys, (void *)mmap->addr, mmap->len / BLOCK_SIZE, 0
            );
        }

        mmap =
            (multiboot_memory_map_t *)((uint64_t)mmap + mmap_tag->entry_size);
    }

    bitmap_mark_region(
        phys, (void *)phys->mem_start, phys->size / BLOCK_SIZE, 1
    );

    phys->used_total = 0;
    phys->used_last = phys->mem_start;
}
