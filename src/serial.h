#ifndef SERIAL_H
#define SERIAL_H

void serial_init();
void serial_deinit();

void serial_putc(char c);
void serial_puts(const char *str);

#endif // SERIAL_H
