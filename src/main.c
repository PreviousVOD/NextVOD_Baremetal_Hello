#include <stdint.h>
#include <stdio.h>

#include "stx7105.h"
#include "stx7105_utils.h"

#define LED_RED_GPIO    PIO0
#define LED_RED_PIN     5U

#define LED_BLUE_GPIO   PIO0
#define LED_BLUE_PIN    4U

#define CONSOLE_ASC     ASC2
#define SYSTEM_DEVID    (0xFE001000U) /* DEVID */
#define SYSTEM_CONFIG34 (0xFE001188U) /* PIO4 */
#define SYSTEM_CONFIG7  (0xFE00111CU) /* RXSEL */

#define MEMTEST_START   0x82000000
#define MEMTEST_END     0x8F000000

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

static void memory_test(void) {
    printf("Begin memory test from 0x%08x to 0x%08x\r\n", MEMTEST_START, MEMTEST_END);

    for (uint32_t i = MEMTEST_START; i < MEMTEST_END; i += 4) {
        *(uint32_t *)i = i;

        if (i % 0x100000 == 0U) {
            printf("Write to 0x%08lx...\r\n", i);
        }
    }

    for (uint32_t i = MEMTEST_START; i < MEMTEST_END; i += 4) {
        if (*(uint32_t *)i != i) {
            printf("Read back error at 0x%08lx\r\n", i);

            return;
        }

        if (i % 0x100000 == 0U) {
            printf("Read from 0x%08lx...\r\n", i);
        }
    }

    printf("Memory test finished.\r\n");
}

int main(void) {
    init_led(LED_RED_GPIO, LED_RED_PIN, 0U);
    init_led(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    uart_init();

    printf("Hello world\r\n");

    __trapa(34); /* TRAPA test code.. */

    memory_test();

    for (;;) {
        set_led(LED_BLUE_GPIO, LED_BLUE_PIN, 1U);
        delay_ms(500);
        set_led(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);
        delay_ms(500);
    }

    return 0;
}

/* tra #34 is SysCall.. */
int syscall_handler(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
    set_led(LED_RED_GPIO, LED_RED_PIN, 1U);
    return 0;
}