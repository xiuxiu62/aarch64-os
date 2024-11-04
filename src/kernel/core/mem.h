#pragma once

#include "core/types.h"

static usize mem_len(const u8 *str) {
  usize len = 0;
  for (; str[len]; len++)
    ;
  return len;
}

// Returns zero if the two strings are the same
static usize mem_cmp(const u8 *lhs, const u8 *rhs) {
  for (; *lhs && (*lhs++ == *rhs++);)
    ;
  return *(unsigned char *)lhs - *(unsigned char *)rhs;
}

// Copy `n` bytes into `dest` from `src`
static void mem_cpy(u8 *dest, const u8 *src, usize n) {
  for (; n--; *dest++ = *src++)
    ;
}

// Sets `n` bytes of `src` to `value`
static void mem_set(u8 *src, u8 value, usize n) {
  for (; n--; *src++ = value)
    ;
}
