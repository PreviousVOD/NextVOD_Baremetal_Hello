    .section .text.init, "ax"
    .global _start

_start:
    nop
    nop
    mov.l stack_k, sp /* Setup R15(SP) */


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


    mov.l main_k,r0
    jsr @r0
    or r0, r0

    mov r0, r4
    mov.l exit_k, r0
    jsr @r0
    or r0, r0

/* It would be more efficient by using indirect addressing instead of 8 instructions... */

    .align 2
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
main_k:
    .long _main /* Same address as main */
exit_k:
    .long _exit
