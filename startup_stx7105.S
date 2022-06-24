/* Startup code, we can't use non-PIC code nor RAM before the LMI is initialized:
 * The CPU starts with physical address 0x0, however the linker is linked to virtual
 * addresses and LMI.
 * Non-PIC code means any indirect addressing other than relative to PC.
 */
    .section .text.init, "ax"
    .global start

start:
    nop
    nop
/* Initialize Blue LED P0_4 */
    mov #16, r0
    mov.l gpio_clr_k, r1
    mov.l r0, @r1
    mov.l gpio_clr_pc0_k, r1
    mov.l r0, @r1
    mov.l gpio_set_pc1_k, r1
    mov.l r0, @r1
    mov.l gpio_clr_pc2_k, r1
    mov.l r0, @r1

/* Initialize Red LED P0_5 */
    mov #32, r0
    mov.l gpio_clr_k, r1
    mov.l r0, @r1
    mov.l gpio_clr_pc0_k, r1
    mov.l r0, @r1
    mov.l gpio_set_pc1_k, r1
    mov.l r0, @r1
    mov.l gpio_clr_pc2_k, r1
    mov.l r0, @r1
    mov.l sr_k, r0
    ldc r0, sr
    mov.l stack_k, sp /* Setup R15(SP) */

/* Turn on Red LED */
    mov #32, r0
    mov.l gpio_set_k, r1
    mov.l r0, @r1

/* TODO: Initialize PMB, setup caches !! */
/* TODO: Switch to 32-bit mode */
/* TODO: Initialize LMI */
    mova init_ram_k, r0 /* The init_ram may be placed too far away, load the pointer to r0 */
    mov.l @r0, r1       /* Load actual function address to r1 */
    add r1, r0          /* address in r1 is relative to init_ram_k */
    jsr @r0             /* Jump to the init_ram function */
    nop

copy_data:
    mov.l sidata_k, r0
    mov.l sdata_k, r1
    mov.l edata_k, r2

loop_copy_data:
    mov.l @r0+, r3 /* Load a word to r3 from [sidata], with post-increment of 4 */
    mov.l r3, @r1  /* Store the word in r3 to [sdata] */
    add #4, r1 /* Increment sdata pointer */
    cmp/ge r1, r2
    bt loop_copy_data

/* Turn on Blue LED */
    mov #16, r0
    mov.l gpio_set_k, r1
    mov.l r0, @r1

zero_bss:
    mov.l edata_k, r0
    mov.l end_k, r1
    mov #0, r2

loop_zero_bss:
    mov.l r2, @r0
    add #4, r0
    cmp/ge r0, r1
    bt loop_zero_bss

setup_fpu:
    mov.l set_fpscr_k, r1
    jsr @r1
    mov #0, r4
    lds r3, fpscr

    mov.l main_k, r0
    jsr @r0
    or r0, r0

    mov r0, r4
    mov.l exit_k, r0
    jsr @r0
    or r0, r0

/* It would be more efficient by using indirect addressing instead of 8 instructions... */
/* Align to 4 byte boundary since we are loading the whole word */
    .align 4
sr_k:
    .long 0x400000F0
gpio_set_k:
    .long 0xFD020004
gpio_clr_k:
    .long 0xFD020008
gpio_clr_pc0_k:
    .long 0xFD020028
gpio_set_pc1_k:
    .long 0xFD020034
gpio_clr_pc2_k:
    .long 0xFD020048
set_fpscr_k:
    .long ___set_fpscr
stack_k:
    .long _stack
sidata_k:
    .long _sidata
sdata_k:
    .long _sdata
edata_k:
    .long _edata
end_k:
    .long _end
init_ram_k:
    .long init_ram
main_k:
    .long _main /* Same address as main */
exit_k:
    .long _exit
