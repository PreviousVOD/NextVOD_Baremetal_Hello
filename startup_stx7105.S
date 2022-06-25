/* Startup code, we can't use non-PIC code nor RAM before the LMI is initialized:
 * The CPU starts with physical address 0x0, however the linker is linked to virtual
 * addresses and LMI.
 * Non-PIC code means any indirect addressing other than relative to PC.
 */
    .section .text.init, "ax"
    .global _start

_start:
    nop
    nop
/* Initialize Blue LED P0_4 */
    mov #16, r0
    mov.l _gpio_clr_k, r1
    mov.l r0, @r1
    mov.l _gpio_clr_pc0_k, r1
    mov.l r0, @r1
    mov.l _gpio_set_pc1_k, r1
    mov.l r0, @r1
    mov.l _gpio_clr_pc2_k, r1
    mov.l r0, @r1

/* Initialize Red LED P0_5 */
    mov #32, r0
    mov.l _gpio_clr_k, r1
    mov.l r0, @r1
    mov.l _gpio_clr_pc0_k, r1
    mov.l r0, @r1
    mov.l _gpio_set_pc1_k, r1
    mov.l r0, @r1
    mov.l _gpio_clr_pc2_k, r1
    mov.l r0, @r1
    mov.l _sr_k, r0
    ldc r0, sr
    mov.l _stack_k, sp /* Setup R15(SP) */

_configure_spinor:
    mov.l _emi_spinor_config_data_k, r0
    mov.l _emi_spinor_config_data_value_k, r1
    mov.l r1, @r0
    mov.l _emi_spinor_modeselect_k, r0
    mov.l _emi_spinor_modeselect_value_k, r1
    mov.l r1, @r0

/* TODO: Initialize PMB, setup caches !! */

/*
 * We need PMBs for LMI and EMI, both cached and uncached.
 * To configure an PMB region, two register writes need to be issued:
 * 1: Write to PMB address array, with region virtual address.
 *    The PMB address array entry is calculated as follows:
 *      - PMB_SLOT_X_ADDR=0xF610_0X00, X=0-F, with a maximum of 16 regions.
 *    The slot contents are defined as follows:
 *      - 0xAA00_0100, AA=Virtual page number, bit 31:30 has to be 0b10
 *        i.e. VA should located in range 0x8000_0000 - 0xBFFF_FFFF
 *        Note: VA needs to align with its configured size.
 * 2: Write to PMB data array, with region size and cache modes.
 *    The PMB data array entry is calculated as follows:
 *    PMB_SLOT_X_DATA=0xF710_0X00, X=0-F, with a maximum of 16 regions.
 */
_invalidate_pmb:
    mov.l _pmb_address_base, r1 /* PMB address slot base address */
    mov #0, r2                  /* Value to be written to PMB address slot */
    mov #1, r3                  /* PMB address stride, 0x100 */
    shll8 r3                    /* See above */
    mov #0, r0                  /* Counter */

_loop_invalid_pmb:
    mov.l r2, @r1   /* Clear slot N */
    add r3, r1      /* Slot += 1 */
    cmp/eq #15, r0  /* Counter == 15? */
    bf/s _loop_invalid_pmb /* Note: this is a delayed branch, be careful. */
    add #1, r0      /* Counter += 1 */

_setup_pmb:
    mova _pmb_poke_start_k, r0
    mov r0, r1
    mova _pmb_poke_end_k, r0
    mov r0, r2

_loop_setup_pmb:
    mov.l @r1+, r0      /* 1st word, register address */
    mov.l @r1+, r3      /* 2nd word, register value   */
    mov.l r3, @r0
    cmp/gt r1, r2
    bt _loop_setup_pmb

_invalidate_caches:
    mov.l _ccn_ccr_k, r0
    mov #0, r1
    mov.l r1, @r0 /* Clear all bits */
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

_setup_caches:
    mov.l _ccn_ccr_value_k, r1
    mov.l r1, @r0
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

_enter_p0:
    mova    _init_lmi, r0
    mov     #0xE0, r1
    shll16  r1
    shll8   r1
    not     r1, r1          /* MASK is 0x1fffffff */
    and     r1, r0          /* unset top 3-bits */
    jmp     @r0
    nop

    .balign 4
/* TODO: Initialize LMI */
_init_lmi:
    mov.l _init_ram_k, r1 /* Load actual function address to r1 */
    jsr @r1               /* Jump to the init_ram function */
    nop

/* TODO: Switch to 32-bit mode */
_enable_se_mode:
    mov.l _ccn_mmucr_k, r0
    mov #4, r1 /* SH4_MMUCR_TI */
    mov.l r1, @r0

    mov.l _ccn_pascr_k, r0
    mov.l _ccn_pascr_value_k, r1
    mov.l r1, @r0

    mova _copy_data, r0
    ldc r0, spc
    stc sr, r0
    ldc r0, ssr
    rte
    nop

/* RTE has to be aligned */
    .align 4
_copy_data:
    mov.l _sidata_k, r0
    mov.l _sdata_k, r1
    mov.l _edata_k, r2

_loop_copy_data:
/* Turn on Red LED */
    mov #32, r7
    mov.l _gpio_set_k, r8
    mov.l r7, @r8

    mov.l @r0+, r3 /* Load a word to r3 from [sidata], with post-increment of 4 */

/* Turn on Blue LED */
    mov #16, r7
    mov.l _gpio_set_k, r8
    mov.l r7, @r8

    mov.l r3, @r1  /* Store the word in r3 to [sdata] */
    add #4, r1     /* Increment sdata pointer */
    cmp/ge r1, r2
    bt _loop_copy_data

_zero_bss:
    mov.l _edata_k, r0
    mov.l _end_k, r1
    mov #0, r2

_loop_zero_bss:
    mov.l r2, @r0
    add #4, r0
    cmp/ge r0, r1
    bt _loop_zero_bss

_setup_fpu:
    mov.l _set_fpscr_k, r1
    jsr @r1
    mov #0, r4
    lds r3, fpscr

    mov.l _main_k, r0
    jsr @r0
    or r0, r0

    mov r0, r4
    mov.l _exit_k, r0
    jsr @r0
    or r0, r0

/* It would be more efficient by using indirect addressing instead of 8 instructions... */
/* Align to 4 byte boundary since we are loading the whole word */
    .align 4

/* PMB address register */
_pmb_address_base:
    .long 0xF6100000

/* PMB poke tables */
_pmb_poke_start_k:
    .long 0xF6100000 /* Address entry #0 register : LMI lower  half,  128MB, mapped to 0x8000_0000 */
    .long 0x80000100 /* Address entry #0 data     : LMI lower  half,  128MB, mapped to 0x8000_0000 */
    .long 0xF6100100 /* Address entry #1 register : LMI higher half,  128MB, mapped to 0x8800_0000 */
    .long 0x88000100 /* Address entry #1 data     : LMI higher half,  128MB, mapped to 0x8800_0000 */
    .long 0xF6100200 /* Address entry #2 register : LMI lower  half,  128MB, mapped to 0x9000_0000 */
    .long 0x90000100 /* Address entry #2 data     : LMI lower  half,  128MB, mapped to 0x9000_0000 */
    .long 0xF6100300 /* Address entry #3 register : LMI higher half,  128MB, mapped to 0x9800_0000 */
    .long 0x98000100 /* Address entry #3 data     : LMI higher half,  128MB, mapped to 0x9800_0000 */
    .long 0xF6100400 /* Address entry #4 register : EMI NOR uncached,  64MB, mapped to 0xA000_0000 */
    .long 0xA0000100 /* Address entry #4 data     : EMI NOR uncached,  64MB, mapped to 0xA000_0000 */
    .long 0xF6100500 /* Address entry #5 register : EMI NOR   cached,  64MB, mapped to 0xA400_0000 */
    .long 0xA4000100 /* Address entry #5 data     : EMI NOR   cached,  64MB, mapped to 0xA400_0000 */
    .long 0xF7100000 /* Data entry #0 register : LMI lower  half,  128MB, uncached, unbuffered */
    .long 0x40000380 /* Data entry #0 data     : LMI lower  half,  128MB, uncached, unbuffered */
    .long 0xF7100100 /* Data entry #1 register : LMI higher half,  128MB, uncached, unbuffered */
    .long 0x48000380 /* Data entry #1 data     : LMI higher half,  128MB, uncached, unbuffered */
    .long 0xF7100200 /* Data entry #2 register : LMI lower  half,  128MB, uncached, unbuffered */
    .long 0x40000380 /* Data entry #2 data     : LMI lower  half,  128MB, uncached, unbuffered */
    .long 0xF7100300 /* Data entry #3 register : LMI higher half,  128MB, uncached, unbuffered */
    .long 0x48000380 /* Data entry #3 data     : LMI higher half,  128MB, uncached, unbuffered */
    .long 0xF7100400 /* Data entry #4 register : EMI NOR uncached,  64MB, uncached, unbuffered */
    .long 0x00000310 /* Data entry #4 data     : EMI NOR uncached,  64MB, uncached, unbuffered */
    .long 0xF7100500 /* Data entry #5 register : EMI NOR   cached,  64MB,   cached,   buffered */
    .long 0x00000118 /* Data entry #5 data     : EMI NOR   cached,  64MB,   cached,   buffered */

_pmb_poke_end_k:
    .long 0x0000000

/* CCN CCR address */
_ccn_ccr_k:
    .long 0xFF00001C
_ccn_ccr_value_k:
    .long 0x8000090D
_ccn_mmucr_k:
    .long 0xFF000010
_ccn_pascr_k:
    .long 0xFF000070
_ccn_pascr_value_k:
    .long 0x80000000

/* SR content */
_sr_k:
    .long 0x400000F0

/* EMI SPI NOR configuration registers */
_emi_spinor_config_data_k:
    .long 0xFE702020
_emi_spinor_config_data_value_k:
    .long 0x00020011
_emi_spinor_modeselect_k:
    .long 0xFE702018
_emi_spinor_modeselect_value_k:
    .long 0x00000002

/* PIO registers for debugging */
_gpio_set_k:
    .long 0xFD020004
_gpio_clr_k:
    .long 0xFD020008
_gpio_clr_pc0_k:
    .long 0xFD020028
_gpio_set_pc1_k:
    .long 0xFD020034
_gpio_clr_pc2_k:
    .long 0xFD020048

/* libc FPU routine */
_set_fpscr_k:
    .long ___set_fpscr

/* C library consts */
_stack_k:
    .long _stack
_sidata_k:
    .long _sidata
_sdata_k:
    .long _sdata
_edata_k:
    .long _edata
_end_k:
    .long _end

/* Function pointers */
_init_ram_k:
    .long _memory_setup_init_ram
_main_k:
    .long _main /* Same address as main */
_exit_k:
    .long _exit
