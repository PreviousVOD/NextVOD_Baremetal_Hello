#include "stx7105.h"
#include "stx7105_utils.h"

void init_led(PIO_TypeDef *gpiox, uint8_t pin, uint8_t init_value) {
    gpiox->CLR_PC0 = 1 << pin;
    gpiox->SET_PC1 = 1 << pin;
    gpiox->CLR_PC2 = 1 << pin;

    set_led(gpiox, pin, init_value);
}

void set_led(PIO_TypeDef *gpiox, uint8_t pin, uint8_t val) {
    if (val) {
        gpiox->SET_POUT = (1 << pin);

    } else {
        gpiox->CLR_POUT = (1 << pin);
    }
}

void delay_ms(uint32_t msec) {
    /* Initialize TMU and count to zero */
    /* TMU clock is from Peripheral clock, approx. 66MHz */
    /* Prescale to 66kHz for convenience (TMUs can only divide by max. 1024) */

    uint32_t reload_value = msec * 66 - 1;

    TMU->TSTR &= ~1U;          /* Stop counter */
    TMU->TCR0  = 0x04U;        /* 1024 prescale */
    TMU->TCNT0 = reload_value; /* 66kHz */
    TMU->TCOR0 = reload_value; /* Reload register */
    TMU->TSTR |= 1U;           /* Start counter */

    /* Wait until underflow occurs */
    uint16_t tcr0 = 0U;
    do {
        tcr0 = TMU->TCR0;
    } while ((tcr0 & 0x100) == 0);

    TMU->TSTR &= ~1U; /* Stop counter */
}