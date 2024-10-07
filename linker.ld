ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

SECTIONS
{
    . = ORIGIN(RAM);
    
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

    . = ALIGN(16);   
    . += 0x10000; /* 1MiB of stack space */
    __stack_top = .;

    /DISCARD/ :
    {
        *(.comment)
        *(.gnu*)
        *(.note*)
        *(.eh_frame*)
    }
}

__bss_size = __bss_end - __bss_start;
