#ifndef STX7105_FDMA_H
#define STX7105_FDMA_H

#include "stx7105.h"

typedef enum {
    FDMA_CMD_START_CHANNEL = 1U,
    FDMA_CMD_PAUSE_CHANNEL = 2U,
    FDMA_CMD_FLUSH_CHANNEL = 3U,
} FDMA_Command_TypeDef;

typedef enum {
    FDMA_STAT_CHANNEL_IDLE    = 0U,
    FDMA_STAT_CHANNEL_RUNNING = 2U,
    FDMA_STAT_CHANNEL_PAUSED  = 3U,
} FDMA_Status_TypeDef;

typedef enum {
    FDMA_REQ_CTL_OP_LDST1  = 0U,
    FDMA_REQ_CTL_OP_LDST2  = 1U,
    FDMA_REQ_CTL_OP_LDST4  = 2U,
    FDMA_REQ_CTL_OP_LDST8  = 3U,
    FDMA_REQ_CTL_OP_LDST16 = 4U,
    FDMA_REQ_CTL_OP_LDST32 = 5U,
} FDMA_ReqCtl_Opcode_TypeDef;

typedef struct {
    __IO uint32_t PTRN;    /* Offset: 0x0000 */
    uint32_t      UNUSED2; /* Offset: 0x0004 */
    __IO uint32_t CNTN;    /* Offset: 0x0008 */
    __IO uint32_t SADDRN;  /* Offset: 0x000C */
    __IO uint32_t DADDRN;  /* Offset: 0x0010 */
} FDMA_FwRegs_Channel_TypeDef;

/* Firmware regs, implemented on the base of DMEM */
typedef struct {
    __IO uint32_t               REVID;         /* Offset: 0x8000 */
    uint32_t                    UNUSED0[1103]; /* Offset: 0x8004 */
    __IO uint32_t               CMD_STATN[16]; /* Offset: 0x9140 */
    __IO uint32_t               REQ_CTLN[16];  /* Offset: 0x9180 */
    uint32_t                    UNUSED1[240];  /* Offset: 0x91C0 */
    FDMA_FwRegs_Channel_TypeDef CHANNELN[16];  /* Offset: 0x9580 */
} FDMA_FWRegs_TypeDef;

#define FDMA_FwRegs_REQ_CTLN_HOLDOFF_Pos 0U
#define FDMA_FwRegs_REQ_CTLN_HOLDOFF_

#define FDMA_FwRegs_REQ_CTLN_OPCODE_Pos 4U
#define FDMA_FwRegs_REQ_CTLN_OPCODE_Msk (0x0FU << FDMA_FwRegs_REQ_CTLN_OPCODE_Pos)

#define FDMA_FwRegs_REQ_CTLN_WNR_Pos 14U
#define FDMA_FwRegs_REQ_CTLN_WNR_Msk (1U << FDMA_FwRegs_REQ_CTLN_INC_ADDR_Pos)

#define FDMA_FwRegs_REQ_CTLN_DATA_SWAP_Pos 17U
#define FDMA_FwRegs_REQ_CTLN_DATA_SWAP_Msk (1U << FDMA_FwRegs_REQ_CTLN_INC_ADDR_Pos)

#define FDMA_FwRegs_REQ_CTLN_INC_ADDR_Pos 21U
#define FDMA_FwRegs_REQ_CTLN_INC_ADDR_Msk (1U << FDMA_FwRegs_REQ_CTLN_INC_ADDR_Pos)

#define FDMA_FwRegs_REQ_CTLN_INITIATOR_Pos 22U
#define FDMA_FwRegs_REQ_CTLN_INITIATOR_Msk (1U << FDMA_FwRegs_REQ_CTLN_INITIATOR_Pos)

#define FDMA_FwRegs_REQ_CTLN_NUM_OPS_Pos 24U
#define FDMA_FwRegs_REQ_CTLN_NUM_OPS_Msk (0xFFU << FDMA_FwRegs_REQ_CTLN_INC_ADDR_Pos)

#endif