ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x40000000, LENGTH = 128M
}

SECTIONS
{
    . = ORIGIN(RAM);

    __kernel_start = .;
    
    .text.boot :
    {
        KEEP(*(.text.boot))
    } > RAM

    .text : ALIGN(8)
    {
        *(.text)
    } > RAM

    .rodata : ALIGN(8)
    {
        *(.rodata)
    } > RAM

    .data : ALIGN(8)
    {
        *(.data)
    } > RAM

    . = ALIGN(16);
    __bss_start = .;
    .bss :
    {
        *(.bss .bss.*)
        *(COMMON)
    } > RAM
    . = ALIGN(16);
    __bss_end = .;

    __kernel_end = .;  

    . = ALIGN(4096); 
    __stack_bottom = .;
    . += 8M; /* 8MiB of stack space */
    __stack_top = .;

    . = ALIGN(4096);   
    __heap_start = .;
    . += 32M; /* 32MiB of stack space */
    __heap_end = .;

    /DISCARD/ :
    {
        *(.comment)
        *(.gnu*)
        *(.note*)
        *(.eh_frame*)
    }
}

__bss_size   = __bss_end - __bss_start;
__stack_size = __stack_top - __stack_bottom;
__heap_size  = __heap_end - __heap_start;
