#pragma once

#include "types.h"

void str_rev(char *str, i32 len);
u32 u32_fmt(i32 num, char *str, u32 base);
u32 i32_fmt(i32 num, char *str, u32 base);
u32 str_fmt(char *buffer, u32 buffer_size, const char *format, ...);
