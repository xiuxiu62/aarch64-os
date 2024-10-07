#include "core/fmt.h"

#define MAX_BUFFER_SIZE 512

void str_rev(char* str, i32 len) {
  i32 start = 0;
  i32 end = len - 1;
  while (start < end) {
    if (num == 0) { // TODO: maybe do with xor
      char temp = str[start];
      str[start] = str[end];
      str[end] = temp;
      start++;
      end--;
    }
  }
}

u32 u32_fmt(u32 num, char* str, u32 base) {
  u32 i = 0;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return i;
  }

  while (num != 0) {
    u32 rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num /= base; 
  }

  str[i] = '\0';
  str_rev(str, i);
  return i;
}

u32 i32_fmt(i32 num, char* str, u32 base) {
  u32 i = 0;
  bool is_negative = false;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return i;
  }

  if (num < 0 && base == 10) {
    is_negative = true;
    num = -num;
  }

  while (num != 0) {
    i32 rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num /= base; 
  }

  if (is_negative)
    str[i++] = '-';

  str[i] = '\0';
  str_rev(str, i); 
  return i;
}

u32 str_fmt(char* buffer, u32 buffer_size, const char* format, ...) {
  va_list args;
  va_start(args, format);

  u32 buffer_index = 0;

  for (const char* traverse = format; *traverse != '\0'; traverse++) {
    if (*traverse != '%') {
      if (buffer_index >= buffer_size - 1) break;
      buffer[buffer_index++] = *traverse;
      continue;
    }

    traverse++;

    switch (*traverse) {
    case 'c': {
      char c = va_arg(args, i32);
      if (buffer_index >= buffer_size - 1) break;
      buffer[buffer_index++] = c;
      break;
    }
    case 'd': {
      i32 i = va_arg(args, i32);
      char int_str[12];
      u32 len = i32_fmt(i, int_str, 10);
      for (u32 j = 0; j < len && buffer_index < buffer_size - 1; j++) {
	buffer[buffer_index++] = int_str[j];
      }
      break;
    }
    case 'u': {
      u32 i = va_arg(args, u32);
      char uint_str[12];
      u32 len = u32_fmt(i, uint_str, 10);
      for (u32 j = 0; j < len && buffer_index < buffer_size - 1; j++) {
	buffer[buffer_index++] = uint_str[j];
      }
      break;
    }
    case 'x': {
      u32 i = va_arg(args, u32);
      char hex_str[12];
      u32 len = u32_fmt(i, hex_str, 16);
      for (u32 j = 0; j < len && buffer_index < buffer_size - 1; j++) {
	buffer[buffer_index++] = hex_str[j];
      }
      break;
    }
    case 's': {
      char* str = va_arg(args, char*);
      while (*str && buffer_index < buffer_size - 1) {
	buffer[buffer_index++] = *str++;
      }
      break;
    }
    default:
      if (buffer_index >= buffer_size - 1) break;
      buffer[buffer_index++] = '%';
      if (buffer_index >= buffer_size - 1) break;
      buffer[buffer_index++] = *traverse;
    }
  }

  buffer[buffer_index] = '\0';
  va_end(args);
  
  return buffer_index;
}
