#include "fs.h"

#include "core/mem.h"

#define FILE_NAME_TOO_LONG  1
#define FILE_ALREADY_EXISTS 2
#define FILE_SYSTEM_FULL    3

static File files[MAX_FILES];

void fs_init(void) {
  usize len = sizeof(File) * MAX_FILES;
  mem_set((u8*)files, 0, len);
}

static File* find_file(const char* name) {
  for (usize i = 0; i < MAX_FILES; i++)
    if (mem_cmp((u8*)files[i].name, (u8*)name) == 0)
      return &files[i];
  return nullptr;
}

u32 fs_create(const char* name) {
  if (mem_len((u8*)name) >= MAX_FILENAME_LENGTH)
    return FILE_NAME_TOO_LONG;

  if (find_file(name))
    return FILE_ALREADY_EXISTS;

  for (usize i = 0; i < MAX_FILES; i++) {
    File* file = &files[i];
    if (file->in_use) continue;
    for (usize j = 0; name[j] != '\0'; j++) { 
      file->name[j] = name[j]; 
      file->name[j] = '\0';
      file->size = 0;
      file->in_use = true;
    }

    return 0;
  }

  return FILE_SYSTEM_FULL; 
}
