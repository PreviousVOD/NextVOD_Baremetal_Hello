
HEAP_SIZE  = 0x10000;
STACK_SIZE = 0x10000;

ENTRY(_start)

/* We don't use 29-bit mode since PMB and LMI initialization has to be done anyway. */

MEMORY {
    EMI         (rx)  : ORIGIN = 0x80000000, LENGTH = 0x01000000 /* EMI copied to Non-cached LMI virtual address: 0x8000_0000 */
    LMI        (rwx)  : ORIGIN = 0x81000000, LENGTH = 0x0F000000 /*               Non-cached LMI virtual address: 0x8100_0000 */
    EMI_CACHED  (rx)  : ORIGIN = 0x90000000, LENGTH = 0x01000000 /*     EMI copied to cached LMI virtual address: 0x9000_0000 */
    LMI_CACHED (rwx)  : ORIGIN = 0x91000000, LENGTH = 0x0F000000 /*                   Cached LMI virtual address: 0x9100_0000 */

}

SECTIONS {
    .text : {
        . = ALIGN(4);
        _stext = .;
        KEEP(*(.text.vtors))
        *(.text.init)
        *(.text.exc)
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >EMI

    _sidata = LOADADDR(.data);

    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >LMI AT >EMI

    _eidata = (_sidata + _edata - _sdata);

    .bss : {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        _end = .;
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