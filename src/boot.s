.section .text.boot
.global _start

_start:
	// Check processor ID is zero (executing on main core)
	mrs	x0, mpidr_el1 
	and	x0, x0, #3
	cbz	x0, 2f

1:	// Halt we're not on the main core 
	wfe
	b	1b

2:	// Build the stack
	ldr 	x0, =__stack_top
	mov	sp, x0

	// Set up memory attrs
	ldr     x0, =0xEE000000000000FF    // Set all memory regions to Normal, Outer Write-Back, Inner Write-Back
	msr     mair_el1, x0
	ldr     x0, =0x0000000000001C5E    // Set up translation control (4K granule, 48-bit VA)
	msr     tcr_el1, x0
	ldr     x0, =0x0000000000000C00    // Enable caches and MMU
	msr     sctlr_el1, x0

	// Enable FPU
	mov	x0, #(3 << 20)
	msr	cpacr_el1, x0

	// Clear the bss section
	ldr 	x0, =__bss_start
	ldr 	x1, =__bss_size
	bl	memzero

	bl	kernel_main
	b	1b 

// x0: address to start clearing, x1: number of bytes to clear
memzero:
	cbz	x1, 2f
1:	// Clear 8 bytes at a time
	str 	xzr, [x0], #8
	subs 	x1, x1, #8
	b.gt	1b
2:	ret
