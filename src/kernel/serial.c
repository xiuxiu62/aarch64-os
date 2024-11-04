#include "serial.h"
#include "core/types.h"

#define UART0_BASE 0x09000000
#define UART0_DR *((volatile u32 *)(UART0_BASE + 0x000))
#define UART0_FR *((volatile u32 *)(UART0_BASE + 0x018))
#define UART0_IBRD *((volatile u32 *)(UART0_BASE + 0x024))
#define UART0_FBRD *((volatile u32 *)(UART0_BASE + 0x028))
#define UART0_LCRH *((volatile u32 *)(UART0_BASE + 0x02C))
#define UART0_CR *((volatile u32 *)(UART0_BASE + 0x030))

void serial_init() {
  /* UART0_CR = 0;    // Disable UART0 */
  /* UART0_IBRD = 26; // 115200 baud */
  /* UART0_FBRD = 3; */
  /* UART0_LCRH = 0x70; // 8 bits, no parity, 1 stop bit */
  UART0_CR = 0x301; // Enable UART0, RXE, TXE
}

void serial_deinit() {
  UART0_CR = 0; // Disable UART0
}

void serial_putc(char c) {
  while (UART0_FR & (1 << 5)) { // Wait for UART to become ready to transmit
  }
  UART0_DR = c;
}

void serial_puts(const char *str) {
  for (usize i = 0; str[i] != '\0'; ++i)
    serial_putc(str[i]);
  serial_putc('\r');
  serial_putc('\n');
}
