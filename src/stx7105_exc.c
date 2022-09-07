#include "stx7105.h"

#define WEAK_ATTR     __attribute__((weak))
#define IRQ_ATTR      __attribute__((interrupt_handler))
#define WEAK_IRQ_ATTR __attribute__((weak, interrupt_handler))

typedef enum {
    EXP_TYPE_RADDERR = 0x0E0,
    EXP_TYPE_WADDERR = 0x100,
    EXP_TYPE_TRAP    = 0x160,
} expevt_type_t;

typedef enum {
    INT_TYPE_TMU_TNUI0  = 0x400,
    INT_TYPE_TMU_TNUI1  = 0x420,
    INT_TYPE_TMU_TNUI2  = 0x440,
    INT_TYPE_TMU_TICPI2 = 0x460,
    INT_TYPE_ASC_UART0  = 0x1160,
    INT_TYPE_ASC_UART1  = 0x1140,
    INT_TYPE_ASC_UART2  = 0x1120,
    INT_TYPE_ASC_UART3  = 0x1100,
} intevt_type_t;

typedef enum {
    TRA_TYPE_START_SCHEDULER = 32,
    TRA_TYPE_YIELD           = 33,
    TRA_TYPE_SYSCALL         = 34,
} tra_type_t;

/* ========================= TMU 0/1/2 Underrun Interrupt Handlers =================================  */

WEAK_ATTR void tuni0_handler(void) {
    /* Does nothing */
}

WEAK_ATTR void tuni1_handler(void) {
    /* Does nothing */
}

WEAK_ATTR void tuni2_handler(void) {
    /* Does nothing */
}

/* ========================= ASC(UART) 0/1/2 Interrupt Handlers =================================  */

WEAK_ATTR void asc0_handler(void) {
    /* Does nothing */
}

WEAK_ATTR void asc1_handler(void) {
    /* Does nothing */
}

WEAK_ATTR void asc2_handler(void) {
    /* Does nothing */
}

WEAK_ATTR void asc3_handler(void) {
    /* Does nothing */
}

/* ========================= Different Trap Code Handlers =================================  */

WEAK_ATTR void syscall_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    /* Does nothing */
}

WEAK_ATTR void yield_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    /* Does nothing */
}

/* ========================= TRAPA(Trap) Exception Handlers =================================  */

WEAK_ATTR void trap_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    tra_type_t tra = (CSP->TRA >> 2U) & 0xFFU; /* TRA[9:2] */

    switch (tra) {
        case TRA_TYPE_SYSCALL:
            syscall_handler(p1, p2, p3, p4);
            break;
        case TRA_TYPE_YIELD:
            yield_handler(p1, p2, p3, p4);
            break;
        default:
            break;
    }
}

/* ========================= Address Error Exception Handlers =================================  */

WEAK_ATTR void radderr_handler(void) {
    /* Dead... */
    for (;;) {
        /* Loop... */
    }
}

WEAK_ATTR void wadderr_handler(void) {
    /* Dead... */
    for (;;) {
        /* Loop... */
    }
}

/* ========================= System Exception Handlers =================================  */

WEAK_IRQ_ATTR void general_exc_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    expevt_type_t expevt = CSP->EXPEVT;
    switch (expevt) {
        case EXP_TYPE_TRAP:
            trap_handler(p1, p2, p3, p4);
            break;
        case EXP_TYPE_RADDERR:
            radderr_handler();
            break;
        case EXP_TYPE_WADDERR:
            wadderr_handler();
            break;
        default:
            break;
    }
}

/* ========================= System Interrupt Handlers =================================  */

WEAK_IRQ_ATTR void general_int_handler(void) {
    intevt_type_t intevt = CSP->INTEVT;
    switch (intevt) {
        case INT_TYPE_TMU_TNUI0:
            tuni0_handler();
            break;
        case INT_TYPE_TMU_TNUI1:
            tuni1_handler();
            break;
        case INT_TYPE_TMU_TNUI2:
            tuni2_handler();
            break;
        case INT_TYPE_ASC_UART0:
            asc0_handler();
            break;
        case INT_TYPE_ASC_UART1:
            asc1_handler();
            break;
        case INT_TYPE_ASC_UART2:
            asc2_handler();
            break;
        case INT_TYPE_ASC_UART3:
            asc3_handler();
            break;
        default:
            break;
    }
}