#include <stdint.h>
#include <stdio.h>

#include "stx7105.h"
#include "stx7105_utils.h"

#define LED_RED_GPIO PIO0
#define LED_RED_PIN  5U

#define LED_BLUE_GPIO PIO0
#define LED_BLUE_PIN  4U

#define CONSOLE_ASC     ASC2
#define SYSTEM_DEVID    (0xFE001000U) /* DEVID */
#define SYSTEM_CONFIG34 (0xFE001188U) /* PIO4 */
#define SYSTEM_CONFIG7  (0xFE00111CU) /* RXSEL */

#define MEMTEST_START 0x82000000
#define MEMTEST_END   0x8F000000

void uart_init(void) {
    PIO4->CLR_PC0 = 1U; /* PC = 110, AFOUT, PP */
    PIO4->SET_PC1 = 1U;
    PIO4->SET_PC2 = 1U;

    *(uint32_t *)SYSTEM_CONFIG34 = 0x00000100UL;    /* BIT[8,0] = 10, AF 3 */
    *(uint32_t *)SYSTEM_CONFIG7 &= ~(0x00000006UL); /* BIT[2:1], UART2 RX SEL */

    CONSOLE_ASC->CTRL     = 0x1509UL; /* 8N1, RX enable, FIFO enable, Baud mode 1 */
    CONSOLE_ASC->BAUDRATE = 0x04B8UL; /* 115200 in baud mode 1, assuming Fcomm=100MHz */
    CONSOLE_ASC->TX_RST   = 0x01UL;   /* Reset TX FIFO, any value OK */
    CONSOLE_ASC->RX_RST   = 0x01UL;   /* Reset RX FIFO, any value OK */
    CONSOLE_ASC->CTRL     = 0x1589UL; /* 8N1, RX enable, FIFO enable, Baud mode 1 */
}

int main(void) {
    init_led(LED_RED_GPIO, LED_RED_PIN, 0U);
    init_led(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    uart_init();

    printf("Hello world\r\n");

    printf("FDMA0 SLIM ID: 0x%08lx\r\n", FDMA0->SLIM_ID);
    printf("FDMA0 SLIM Version: 0x%08lx\r\n", FDMA0->SLIM_VER);

    printf("Dumping FDMA0 SLIM DMEM@%p: \r\n", FDMA0->SLIM_DMEM);

    for (uint32_t i = 0; i < 2048; i++) {
        if (i % 8 == 0) {
            printf("0x%04lx: ", i * 4);
        }

        printf("0x%08lx ", ((uint32_t *)FDMA0->SLIM_DMEM)[i]);

        if (i % 8 == 7) {
            printf("\r\n");
        }
    }

    printf("Dumping FDMA0 SLIM IMEM @%p: \r\n", FDMA0->SLIM_IMEM);

    for (uint32_t i = 0; i < 4096; i++) {
        if (i % 8 == 0) {
            printf("0x%04lx: ", i * 4);
        }

        printf("0x%08lx ", ((uint32_t *)FDMA0->SLIM_IMEM)[i]);

        if (i % 8 == 7) {
            printf("\r\n");
        }
    }

    delay_ms(5000);

    for (;;) {
        set_led(LED_BLUE_GPIO, LED_BLUE_PIN, 1U);
        delay_ms(500);
        set_led(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);
        delay_ms(500);
    }

    return 0;
}
