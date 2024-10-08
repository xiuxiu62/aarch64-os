#pragma once

#include "core/types.h"
#include "core/mem.h"

#define MAX_FILENAME_LENGTH 64
#define MAX_FILENAME_SIZE   1024
#define MAX_FILES           128        

typedef struct {
  char* name;
  usize size;
  u8*   content;
  bool  in_use;
} File;

void fs_init(void);
void fs_deinit(void);
u32 fs_create(const char* name);
u32 fs_write(const char* name, const u8* data, usize size);
u32 fs_read(const char* name, u8* buffer, usize size);
u32 fs_delete(const char* name);
void fs_list(void);
