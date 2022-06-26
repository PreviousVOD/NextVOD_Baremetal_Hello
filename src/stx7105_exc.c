#include "stx7105.h"

#define EXPEVT_BASE 0xFF000024
#define TRA_BASE    0xFF000020

typedef enum {
    EXP_TYPE_TRAP = 0x160,
} expevt_type_t;

typedef enum {
    TRA_TYPE_SYSCALL = 34,
} tra_type_t;

static int uart_write(char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        while (ASC2->STA & 0x200) {
            /* TX FIFO full... */
        }
        ASC2->TX_BUF = (uint8_t)ptr[i];
    }

    return len;
}

static int syscall_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    if (p1 == 4) {
        return uart_write((char *)p3, (int)p4);
    }

    return 0;
}

static int trap_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    tra_type_t tra = *(uint32_t *)TRA_BASE;

    switch (tra) {
        case TRA_TYPE_SYSCALL:
            return syscall_handler(p1, p2, p3, p4);
            break;
        default:
            break;
    }

    return 0;
}

__attribute__((interrupt_handler)) int general_exc_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    expevt_type_t expevt = *(uint32_t *)EXPEVT_BASE;

    PIO0->SET_POUT = (1 << 5);

    switch (expevt) {
        case EXP_TYPE_TRAP:
            trap_handler(p1, p2, p3, p4);
            break;
        default:
            break;
    }

    return 0;
}