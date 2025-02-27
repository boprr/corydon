#include "mem/bitmap.h"

void bitmap_mark(mem_bitmap *map, size_t block, int value) {
    size_t addr = block / BITS_PER_BYTE;
    size_t offset = block % BITS_PER_BYTE;
    if (value)
        map->map[addr] |= (1 << offset);
    else
        map->map[addr] &= ~(1 << offset);
}

int bitmap_get(mem_bitmap *map, size_t block) {
    size_t addr = block / BITS_PER_BYTE;
    size_t offset = block % BITS_PER_BYTE;
    return map->map[addr] & (1 << offset);
}

void bitmap_mark_blocks(
    mem_bitmap *map, size_t start, size_t size_block, int value
) {
    if (!value) {
        if (start < map->used_last) map->used_last = start;
        map->used_total += size_block * BLOCK_SIZE;
    } else
        map->used_total -= size_block * BLOCK_SIZE;
    for (size_t i = 0; i < size_block; i++) bitmap_mark(map, i + start, value);
}

void bitmap_mark_region(mem_bitmap *map, void *base, size_t size, int value) {
    size_t t_base;
    size_t t_size;
    t_base = bitmap_ptr_to_block(map, base);
    t_size = math_divroundup(size, BLOCK_SIZE);
    bitmap_mark_blocks(map, t_base, t_size, value);
}

size_t bitmap_ptr_to_block(mem_bitmap *map, void *ptr) {
    uint8_t *ptr_u8 = (uint8_t *)ptr;
    return (size_t)(ptr_u8 - map->mem_start) / BLOCK_SIZE;
}

void *bitmap_block_to_ptr(mem_bitmap *map, size_t block) {
    uint8_t *ptr = (uint8_t *)(map->mem_start + (block * BLOCK_SIZE));
    return (void *)ptr;
}

size_t bitmap_find_free_region(mem_bitmap *map, size_t blocks) {
    size_t start = map->used_last;
    size_t size = 0;

    for (size_t i = start; i < (map->size / BLOCK_SIZE); i++) {
        if (bitmap_get(map, i)) {
            size = 0;
            start = i + 1;
            continue;
        }
        if (blocks == 1) map->used_last = start + 1;
        size++;
        if (size >= blocks) return start;
    }

    return BLOCK_NOAVIL;
}

void *bitmap_alloc(mem_bitmap *map, size_t blocks) {
    size_t region = bitmap_find_free_region(map, blocks);
    if (region == BLOCK_NOAVIL) {
        panic("MEMORY IS FULL");
    }
    bitmap_mark_blocks(map, region, blocks, 1);
    return bitmap_block_to_ptr(map, region);
}

void bitmap_free(mem_bitmap *map, void *base, size_t block) {
    bitmap_mark_region(map, base, block * BLOCK_SIZE, 0);
}
