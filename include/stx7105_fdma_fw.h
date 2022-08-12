#ifndef STX7105_FDMA_FW_H
#define STX7105_FDMA_FW_H

#include <stdint.h>

typedef struct {
    uint32_t entry_point;
    uint32_t text_size;
    uint32_t data_size;
    uint8_t *text;
    uint8_t *data;
} stx7105_fdma_fw_t;

extern stx7105_fdma_fw_t stx7105_fdma0_fw;
extern stx7105_fdma_fw_t stx7105_fdma1_fw;

#endif