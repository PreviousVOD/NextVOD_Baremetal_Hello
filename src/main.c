#include <stdint.h>

#define ST_GPIO0_BASE_ADDR       (0xFD020000)
#define ST_GPIO0_OFFSET_POUT     0x00U
#define ST_GPIO0_OFFSET_SET_POUT 0x04U
#define ST_GPIO0_OFFSET_CLR_POUT 0x08U
#define ST_GPIO0_OFFSET_PC0      0x20U
#define ST_GPIO0_OFFSET_SET_PC0  0x24U
#define ST_GPIO0_OFFSET_CLR_PC0  0x28U
#define ST_GPIO0_OFFSET_PC1      0x30U
#define ST_GPIO0_OFFSET_SET_PC1  0x34U
#define ST_GPIO0_OFFSET_CLR_PC1  0x38U
#define ST_GPIO0_OFFSET_PC2      0x40U
#define ST_GPIO0_OFFSET_SET_PC2  0x44U
#define ST_GPIO0_OFFSET_CLR_PC2  0x48U

#define SH_TMU_BASE_ADDR    (0xFFD80000)
#define SH_TMU_OFFSET_TOCR  0x00U
#define SH_TMU_OFFSET_TSTR  0x04U
#define SH_TMU_OFFSET_TCOR0 0x08U
#define SH_TMU_OFFSET_TCNT0 0x0CU
#define SH_TMU_OFFSET_TCR0  0x10U
#define SH_TMU_OFFSET_TCOR1 0x14U
#define SH_TMU_OFFSET_TCNT1 0x18U
#define SH_TMU_OFFSET_TCR1  0x1CU
#define SH_TMU_OFFSET_TCOR2 0x20U
#define SH_TMU_OFFSET_TCNT2 0x24U
#define SH_TMU_OFFSET_TCR2  0x28U
#define SH_TMU_OFFSET_TCPR2 0x2CU

#define LED_RED_PIN  5U
#define LED_BLUE_PIN 4U

static void init_led(uint8_t pin) {
    *(uint32_t *)(ST_GPIO0_BASE_ADDR + ST_GPIO0_OFFSET_CLR_PC0) = (1 << pin);
    *(uint32_t *)(ST_GPIO0_BASE_ADDR + ST_GPIO0_OFFSET_SET_PC1) = (1 << pin);
    *(uint32_t *)(ST_GPIO0_BASE_ADDR + ST_GPIO0_OFFSET_CLR_PC2) = (1 << pin);
}

static void set_led(uint8_t pin, uint8_t val) {
    if (val) {
        *(uint32_t *)(ST_GPIO0_BASE_ADDR + ST_GPIO0_OFFSET_SET_POUT) = (1 << pin);

    } else {
        *(uint32_t *)(ST_GPIO0_BASE_ADDR + ST_GPIO0_OFFSET_CLR_POUT) = (1 << pin);
    }
}

static void delay_ms(uint32_t msec) {
    /* Initialize TMU and count to zero */
    /* TMU clock is from Peripheral clock, approx. 66MHz */
    /* Prescale to 450kHz for convenience (TMUs can only divide by max. 1024) */

    *(uint8_t *)(SH_TMU_BASE_ADDR + SH_TMU_OFFSET_TSTR) &= ~1U;          /* Stop counter */
    *(uint16_t *)(SH_TMU_BASE_ADDR + SH_TMU_OFFSET_TCR0)  = 0x04U;       /* 1024 prescale */
    *(uint32_t *)(SH_TMU_BASE_ADDR + SH_TMU_OFFSET_TCNT0) = (msec * 66); /* 66kHz */
    *(uint8_t *)(SH_TMU_BASE_ADDR + SH_TMU_OFFSET_TSTR) |= 1U;           /* Start counter */

    /* Wait until underflow occurs */
    uint16_t tcr0 = 0U;
    do {
        tcr0 = *(uint16_t *)(SH_TMU_BASE_ADDR + SH_TMU_OFFSET_TCR0);
    } while ((tcr0 & 0x100) == 0);

    *(uint8_t *)(SH_TMU_BASE_ADDR + SH_TMU_OFFSET_TSTR) &= ~1U; /* Stop counter */
}

int main(void) {
    init_led(LED_RED_PIN);
    init_led(LED_BLUE_PIN);

    set_led(LED_RED_PIN, 0);
    set_led(LED_BLUE_PIN, 0);

    for (;;) {
        /* Dead loop */
        set_led(LED_BLUE_PIN, 1);
        delay_ms(1);
        set_led(LED_BLUE_PIN, 0);
        delay_ms(1);
    }
}