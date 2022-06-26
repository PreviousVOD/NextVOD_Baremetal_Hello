#include <stdint.h>
#include <stdio.h>

#include "stx7105.h"

#define LED_RED_GPIO PIO0
#define LED_RED_PIN  5U

#define LED_BLUE_GPIO PIO0
#define LED_BLUE_PIN  4U

#define CONSOLE_ASC     ASC2
#define SYSTEM_DEVID    (0xFE001000U) /* DEVID */
#define SYSTEM_CONFIG34 (0xFE001188U) /* PIO4 */
#define SYSTEM_CONFIG7  (0xFE00111CU) /* RXSEL */

static void set_led(PIO_TypeDef *gpiox, uint8_t pin, uint8_t val);

static void init_led(PIO_TypeDef *gpiox, uint8_t pin, uint8_t init_value) {
    gpiox->CLR_PC0 = 1 << pin;
    gpiox->SET_PC1 = 1 << pin;
    gpiox->CLR_PC2 = 1 << pin;

    set_led(gpiox, pin, init_value);
}

static void set_led(PIO_TypeDef *gpiox, uint8_t pin, uint8_t val) {
    if (val) {
        gpiox->SET_POUT = (1 << pin);

    } else {
        gpiox->CLR_POUT = (1 << pin);
    }
}

static void delay_ms(uint32_t msec) {
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

static void uart_init(void) {
    // PIO4->CLR_PC0 = 1U; /* PC = 110, AFOUT, PP */
    // PIO4->SET_PC1 = 1U;
    // PIO4->SET_PC2 = 1U;

    // *(uint32_t *)SYSTEM_CONFIG34 = 0x00000100UL;  /* BIT[8,0] = 10, AF 3 */
    // *(uint32_t *)SYSTEM_CONFIG7 &= ~(0x00000006UL); /* BIT[2:1], UART2 RX SEL */

    // CONSOLE_ASC->CTRL     = 0x1509UL; /* 8N1, RX enable, FIFO enable, Baud mode 1 */
    // CONSOLE_ASC->BAUDRATE = 0x04B8UL; /* 115200 in baud mode 1, assuming Fcomm=100MHz */
    // CONSOLE_ASC->TX_RST   = 0x01UL;   /* Reset TX FIFO, any value OK */
    // CONSOLE_ASC->RX_RST   = 0x01UL;   /* Reset RX FIFO, any value OK */
    // CONSOLE_ASC->CTRL     = 0x1589UL; /* 8N1, RX enable, FIFO enable, Baud mode 1 */
}

int main(void) {
    // init_led(LED_RED_GPIO, LED_RED_PIN, 0U);
    // init_led(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);

    // if(*(uint32_t *)SYSTEM_DEVID != 0x2D43E041UL) {
    //     set_led(LED_RED_GPIO, LED_RED_PIN, 1U);
    // }

    uart_init();

    // for (uint16_t i = 0; i < 65534; i++) {
    //     CONSOLE_ASC->TX_BUF = i;
    //     while (CONSOLE_ASC->STA & 0x200) {
    //         /**/
    //     }
    // }

    // printf("Hello world\r\n");

    for (;;) {
        /* Dead loop */
        // set_led(LED_BLUE_GPIO, LED_BLUE_PIN, 1);
        // delay_ms(1000);
        // set_led(LED_BLUE_GPIO, LED_BLUE_PIN, 0);
        // delay_ms(1000);
    }

    return 0;
}