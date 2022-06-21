
HEAP_SIZE  = 0x10000;
STACK_SIZE = 0x10000;

ENTRY(_start)

/* For bare-metal applications, we use 29-bit addressing mode. */

MEMORY {
    EMI  (rx) : ORIGIN = 0x00000000, LENGTH = 0x00100000 /* EMI physical address: 0x0000_0000 */
    LMI (rwx) : ORIGIN = 0x0C000000, LENGTH = 0x10000000 /* LMI physical address: 0x0C00_0000 */
}

SECTIONS {
    .text : {
        . = ALIGN(4);
        *(.text.init)
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
    } >EMI

    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
    } >LMI AT >EMI

    _sidata = LOADADDR(.data);

    .bss : {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        _end = ALIGN(8);
        __end = _end;
    } >LMI

    .heap : {
        . = ALIGN(8);
        . += HEAP_SIZE;
    }

    .stack : {
        . = ALIGN(8);
        . += STACK_SIZE;
    }

    _stack = ORIGIN(LMI) + LENGTH(LMI);
    PROVIDE(__stack = _stack);
}