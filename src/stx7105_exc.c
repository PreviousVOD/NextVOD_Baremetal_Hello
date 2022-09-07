#include "stx7105.h"

#define WEAK_ATTR     __attribute__((weak))
#define IRQ_ATTR      __attribute__((interrupt_handler))
#define WEAK_IRQ_ATTR __attribute__((weak, interrupt_handler))

typedef enum {
    EXP_TYPE_TRAP = 0x160,
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
    TRA_TYPE_SYSCALL = 34,
} tra_type_t;

/* ========================= TMU 0/1/2 Underrun Interrupt Handlers =================================  */

WEAK_ATTR int tuni0_handler(void) {
    /* Does nothing */
    return 0;
}

WEAK_ATTR int tuni1_handler(void) {
    /* Does nothing */
    return 0;
}

WEAK_ATTR int tuni2_handler(void) {
    /* Does nothing */
    return 0;
}

/* ========================= ASC(UART) 0/1/2 Interrupt Handlers =================================  */

WEAK_ATTR int asc2_handler(void) {
    /* Does nothing */
    return 0;
}

/* ========================= Different Trap Code Handlers =================================  */

WEAK_ATTR int syscall_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    return 0;
}

/* ========================= TRAPA(Trap) Exception Handlers =================================  */

WEAK_ATTR int trap_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    tra_type_t tra = (CSP->TRA >> 2U) & 0xFFU; /* TRA[9:2] */

    switch (tra) {
        case TRA_TYPE_SYSCALL:
            return syscall_handler(p1, p2, p3, p4);
            break;
        default:
            break;
    }

    return 0;
}

/* ========================= System Exception Handlers =================================  */

WEAK_IRQ_ATTR int general_exc_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    expevt_type_t expevt = CSP->EXPEVT;
    switch (expevt) {
        case EXP_TYPE_TRAP:
            return trap_handler(p1, p2, p3, p4);
            break;
        default:
            break;
    }

    return 0;
}

/* ========================= System Interrupt Handlers =================================  */

WEAK_IRQ_ATTR int general_int_handler(void) {
    intevt_type_t intevt = CSP->INTEVT;
    switch (intevt) {
        case INT_TYPE_TMU_TNUI0:
            return tuni0_handler();
            break;
        case INT_TYPE_TMU_TNUI1:
            return tuni1_handler();
            break;
        case INT_TYPE_TMU_TNUI2:
            return tuni2_handler();
            break;
        case INT_TYPE_ASC_UART2:
            return asc2_handler();
            break;
        default:
            break;
    }
    return 0;
}