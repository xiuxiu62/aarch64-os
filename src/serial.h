#pragma once

void serial_init();
void serial_deinit();

void serial_putc(char c);
void serial_puts(const char *str);
