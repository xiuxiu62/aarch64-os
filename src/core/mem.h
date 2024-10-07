#pragma once

#include "types.h"

usize str_len(const char* str) {
  usize len = 0;
  for (; str[len]; len++);
  return len;
}

usize str_cmp(const char* lhs, const char* rhs) {
  for (;*lhs && (*lhs++ == *rhs++););
  return *(unsigned char*)lhs - *(unsigned char*)rhs; 
}
