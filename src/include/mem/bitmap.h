#ifndef BITMAP_H
#define BITMAP_H

#include <stddef.h>
#include <stdint.h>

#include "utils.h"

#define BLOCK_SIZE 4096
#define BITS_PER_BYTE 8

#define BLOCK_VALID 0
#define BLOCK_INVALID -1
#define BLOCK_NOAVIL 1

typedef struct {
    uintptr_t mem_start;
    uint64_t size;
    uint8_t *map;

    uintptr_t used_last;
    uint64_t used_total;
} mem_bitmap;

int bitmap_get(mem_bitmap *map, size_t block);

void bitmap_mark_blocks(
    mem_bitmap *map, size_t start, size_t size_block, int value
);
void bitmap_mark(mem_bitmap *map, size_t block, int value);

void bitmap_mark_region(mem_bitmap *map, void *base, size_t size, int value);

size_t bitmap_ptr_to_block(mem_bitmap *map, void *ptr);
void *bitmap_block_to_ptr(mem_bitmap *map, size_t block);

size_t bitmap_find_free_region(mem_bitmap *map, size_t blocks);

void *bitmap_alloc(mem_bitmap *map, size_t blocks);
void bitmap_free(mem_bitmap *map, void *base, size_t block);
#endif
