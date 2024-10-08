#include "core/fmt.h"
#include "serial.h"
#include "core/alloc.h"
#include "core/mem.h"
#include "fs.h"

void kernel_halt();

void kernel_main() {
  serial_init();
  fs_init();

  u32 result = fs_create("example.txt");
  result = fs_create("example.txt");
  char rb; 
  u32_fmt(result, &rb, 10);
  serial_puts(&rb);

  /* char buf[64]; */
  /* str_fmt(buf, 64, "(x: %d, y: %d)", 10, 20); */
  /* serial_puts(buf); */

  // char buf[16];
  // u32_fmt(125, buf, 10);
  // serial_puts(buf);
  // u32 u32_fmt(i32 num, char *str, u32 base);

  char* temp = (char*)alloc(16);
  char* temp2 = (char*)alloc(16);
  temp  = "hey sailor!";
  temp2 = "hii sailor?";

  usize temp3 = mem_cmp((u8*)temp, (u8*)temp2);
  char len_buf[4];
  u32_fmt(temp3, len_buf, 10);
  serial_puts(len_buf);
  serial_puts(temp);
  serial_puts(temp2);

  /* serial_puts("hey sailor"); */
  serial_puts("☠️");
  serial_puts("hey sailor");

  kernel_halt();
}

void kernel_halt() {
  while (1)
    asm volatile("wfe");
}
