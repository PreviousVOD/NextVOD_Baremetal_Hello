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

#define LED_RED_PIN  4U
#define LED_BLUE_PIN 5U

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

static void delay(uint32_t msec) {
    volatile uint32_t loop = 0;
    for (uint32_t i = 0; i < msec; i++) {
        for (uint32_t j = 0; j < 40000; i++) {
            loop++;
        }
    }
}

int main(void) {
    init_led(LED_RED_PIN);
    init_led(LED_BLUE_PIN);

    set_led(LED_RED_PIN, 1);
    set_led(LED_BLUE_PIN, 1);

    for (;;) {
        /* Dead loop */
        set_led(LED_BLUE_PIN, 1);
        delay(1000);
        set_led(LED_BLUE_PIN, 0);
        delay(1000);
    }
}