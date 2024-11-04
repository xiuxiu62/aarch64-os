#pragma once

typedef struct {
  char* next_arg;
} va_list;

#define va_start(ap, last) ((void)((ap).next_arg = (char*)&(last) + sizeof(last)))

#define va_arg(ap, type) (*(type*)((ap).next_arg += sizeof(type), (ap).next_arg - sizeof(type)))

#define va_end(ap) (nullptr)
