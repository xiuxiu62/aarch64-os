#pragma once

#include "types.h"

#define nullptr ((void*)0);

extern u8 __heap_start[];
extern u8 __heap_end[];

static u8* heap_ptr = (u8*)__heap_start;

void* alloc(usize size) {
  size = (size + 7) & -7;

  if (heap_ptr + size > (u8*)__heap_end)
    return nullptr;

  void* region = (void*)heap_ptr;
  heap_ptr += size;

  return region;
}
