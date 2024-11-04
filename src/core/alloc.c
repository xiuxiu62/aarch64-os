#include "core/alloc.h"

extern u8 __heap_start[];
extern u8 __heap_end[];

static usize MINIMUM_BLOCK_SIZE = sizeof(Block);

static Allocator global_allocator;

void allocator_init(Allocator *allocator) {
  Block **free_list = &allocator->free_list;
  *free_list = (Block *)__heap_start;
  (*free_list)->size = (usize)(__heap_end - __heap_start);
  (*free_list)->next = nullptr;
  (*free_list)->is_free = true;
}

usize align(usize size) { return (size + 7) & -8; }

void *block_to_payload(Block *block) { return (void *)(block + 1); }

Block *payload_to_block(void *payload) { return ((Block *)payload) - 1; }

Block *find_best_fit(Allocator *allocator, usize size) {
  Block *best_fit = nullptr, *current = allocator->free_list;

  while (current != nullptr) {
    if (current->is_free && current->size >= size) {
      if (best_fit == nullptr || current->size < best_fit->size) {
        best_fit = current;
      }
    }
    current = current->next;
  }

  return best_fit;
}

void split_block(Block *block, usize size) {
  if (block->size >= size + MINIMUM_BLOCK_SIZE + sizeof(Block)) {
    Block *new_block = (Block *)((u8 *)block + size);
    new_block->size = block->size - size;
    new_block->is_free = true;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
  }
}

void coalesce(Allocator *allocator) {
  Block *current = allocator->free_list;

  while (current != nullptr && current->next != nullptr) {
    if (current->is_free && current->next->is_free &&
        (u8 *)current + current->size == (u8 *)current->next) {
      current->size += current->next->size;
      current->next = current->next->next;
    } else {
      current = current->next;
    }
  }
}

void global_allocator_init() { allocator_init(&global_allocator); }

void *alloc(usize requested_size) {
  if (requested_size == 0)
    return nullptr;

  usize total_size = align(requested_size + sizeof(Block));
  Block *block = find_best_fit(&global_allocator, total_size);
  if (block == nullptr)
    return nullptr;

  split_block(block, total_size);
  block->is_free = false;

  return block_to_payload(block);
}

void free(void *ptr) {
  if (ptr == nullptr)
    return;

  Block *block = payload_to_block(ptr);
  block->is_free = true;

  coalesce(&global_allocator);
}
