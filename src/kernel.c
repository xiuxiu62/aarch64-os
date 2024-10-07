typedef char      i8;
typedef short     i16;
typedef long      i32;
typedef long long i64;
typedef long long isize;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned long      u32;
typedef unsigned long long u64;
typedef unsigned long long usize;

typedef float  f32;
typedef double f64;

#define UART0_BASE 0x09000000
#define UART0_DR *((volatile u32 *)(UART0_BASE + 0x000))
#define UART0_FR *((volatile u32 *)(UART0_BASE + 0x018))
#define UART0_IBRD *((volatile u32 *)(UART0_BASE + 0x024))
#define UART0_FBRD *((volatile u32 *)(UART0_BASE + 0x028))
#define UART0_LCRH *((volatile u32 *)(UART0_BASE + 0x02C))
#define UART0_CR *((volatile u32 *)(UART0_BASE + 0x030))

void kernel_halt();
/* void uart_init(); */
void uart_putc(char c);
void uart_puts(const char *str);

void kernel_main() {
  uart_putc('h');
  uart_putc('e');
  uart_putc('y');
  uart_putc('\n');

  /* uart_init(); */
  uart_puts("hey sailor\r\n");

  kernel_halt();
}

/* void uart_init() { */
/*   UART0_CR = 0;    // Disable UART0 */
/*   UART0_IBRD = 26; // 115200 baud */
/*   UART0_FBRD = 3; */
/*   UART0_LCRH = 0x70; // 8 bits, no parity, 1 stop bit */
/*   UART0_CR = 0x301;  // Enable UART0, RXE, TXE */
/* } */

void uart_putc(char c) {
  while (UART0_FR & (1 << 5)) { // Wait for UART to become ready to transmit
  }
  UART0_DR = c;
}

void uart_puts(const char *str) {
  for (usize i = 0; str[i] != '\0'; ++i)
    uart_putc(str[i]);
}

void kernel_halt() {
  while (1)
    asm volatile("wfe");
}

