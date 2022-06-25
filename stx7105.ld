
HEAP_SIZE  = 0x10000;
STACK_SIZE = 0x10000;

ENTRY(_start)

/* We don't use 29-bit mode since PMB and LMI initialization has to be done anyway. */

MEMORY {
    EMI  (rx) : ORIGIN = 0xA0000000, LENGTH = 0x00100000 /* EMI virtual address: 0xA000_0000 */
    LMI (rwx) : ORIGIN = 0x80000000, LENGTH = 0x10000000 /* LMI virtual address: 0x8000_0000 */
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