#pragma once

#include "core/types.h"

typedef struct Block {
  usize size;         // Size of the block including header
  struct Block *next; // Next free block in the list
  bool is_free;       // Whether this block is free
} Block;

typedef struct {
  Block *free_list;
} Allocator;

void allocator_init(Allocator *allocator);

void global_allocator_init();

void *alloc(usize requested_size);

void free(void *ptr);
