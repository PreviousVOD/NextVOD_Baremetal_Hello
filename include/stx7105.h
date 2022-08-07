#ifndef STX7105_H
#define STX7105_H

#include <stdint.h>

#define __PACKED __attribute__((packed, aligned(1)))
#define __IO     volatile

typedef struct {
    __IO uint8_t POUT;        /* Offset: 0x00, GPIO pin output register */
    __IO uint8_t UNUSED0[3];  /* Offset: 0x01 */
    __IO uint8_t SET_POUT;    /* Offset: 0x04, GPIO pin output set register */
    __IO uint8_t UNUSED1[3];  /* Offset: 0x05 */
    __IO uint8_t CLR_POUT;    /* Offset: 0x08, GPIO pin output clear register */
    __IO uint8_t UNUSED2[7];  /* Offset: 0x09 */
    __IO uint8_t PIN;         /* Offset: 0x10, GPIO pin input register */
    __IO uint8_t UNUSED3[15]; /* Offset: 0x11 */
    __IO uint8_t PC0;         /* Offset: 0x20, GPIO pin config register 0 */
    __IO uint8_t UNUSED4[3];  /* Offset: 0x21 */
    __IO uint8_t SET_PC0;     /* Offset: 0x24, GPIO pin config set register 0 */
    __IO uint8_t UNUSED5[3];  /* Offset: 0x25 */
    __IO uint8_t CLR_PC0;     /* Offset: 0x28, GPIO pin config clear register 0 */
    __IO uint8_t UNUSED6[7];  /* Offset: 0x29 */
    __IO uint8_t PC1;         /* Offset: 0x30, GPIO pin config register 1 */
    __IO uint8_t UNUSED7[3];  /* Offset: 0x31 */
    __IO uint8_t SET_PC1;     /* Offset: 0x34, GPIO pin config set register 1 */
    __IO uint8_t UNUSED8[3];  /* Offset: 0x35 */
    __IO uint8_t CLR_PC1;     /* Offset: 0x38, GPIO pin config clear register 1 */
    __IO uint8_t UNUSED9[7];  /* Offset: 0x39 */
    __IO uint8_t PC2;         /* Offset: 0x40, GPIO pin config register 2 */
    __IO uint8_t UNUSED10[3]; /* Offset: 0x41 */
    __IO uint8_t SET_PC2;     /* Offset: 0x44, GPIO pin config set register 2 */
    __IO uint8_t UNUSED11[3]; /* Offset: 0x45 */
    __IO uint8_t CLR_PC2;     /* Offset: 0x48, GPIO pin config clear register 2 */
    __IO uint8_t UNUSED12[7]; /* Offset: 0x49 */
    __IO uint8_t PCOMP;       /* Offset: 0x50, GPIO pin input comparison register */
    __IO uint8_t UNUSED13[3]; /* Offset: 0x51 */
    __IO uint8_t SET_PCOMP;   /* Offset: 0x54, GPIO pin input comparison set regiser */
    __IO uint8_t UNUSED14[3]; /* Offset: 0x55 */
    __IO uint8_t CLR_PCOMP;   /* Offset: 0x58, GPIO pin input comparison clear regiser */
    __IO uint8_t UNUSED15[7]; /* Offset: 0x59 */
    __IO uint8_t PMASK;       /* Offset: 0x60, GPIO pin input comparison mask register */
    __IO uint8_t UNUSED16[3]; /* Offset: 0x61 */
    __IO uint8_t SET_PMASK;   /* Offset: 0x64, GPIO pin input comparison mask set regiser */
    __IO uint8_t UNUSED17[3]; /* Offset: 0x65 */
    __IO uint8_t CLR_PMASK;   /* Offset: 0x68, GPIO pin input comparison mask clear regiser */
} PIO_TypeDef;

typedef struct {
    __IO uint32_t BAUDRATE;  /* Offset: 0x00, ASCn baud rate generator register */
    __IO uint32_t TX_BUF;    /* Offset: 0x04, ASCn transmit buffer register */
    __IO uint32_t RX_BUF;    /* Offset: 0x08, ASCn receive buffer register */
    __IO uint32_t CTRL;      /* Offset: 0x0C, ASCn control register */
    __IO uint32_t INT_EN;    /* Offset: 0x10, ASCn interrupt enable register */
    __IO uint32_t STA;       /* Offset: 0x14, ASCn interrupt status register */
    __IO uint32_t GUARDTIME; /* Offset: 0x18, ASCn guard time register */
    __IO uint32_t TIMEOUT;   /* Offset: 0x1C, ASCn time out register */
    __IO uint32_t TX_RST;    /* Offset: 0x20, ASCn transmit FIFO reset register */
    __IO uint32_t RX_RST;    /* Offset: 0x24, ASCn receive FIFO reset register */
    __IO uint32_t RETRIES;   /* Offset: 0x28, ASCn number of retries on transmission register */
} ASC_TypeDef;

typedef struct {
    __IO uint8_t  TOCR;       /* Offset: 0x00, Timer output control register */
    uint8_t       UNUSED0[3]; /* Offset: 0x01 */
    __IO uint8_t  TSTR;       /* Offset: 0x04, Timer start register */
    uint8_t       UNUSED1[3]; /* Offset: 0x05 */
    __IO uint32_t TCOR0;      /* Offset: 0x08, Timer constant register 0 */
    __IO uint32_t TCNT0;      /* Offset: 0x0C, Timer counter 0 */
    __IO uint16_t TCR0;       /* Offset: 0x10, Timer control register 0 */
    uint8_t       UNUSED2[2]; /* Offset: 0x11 */
    __IO uint32_t TCOR1;      /* Offset: 0x14, Timer constant register 1 */
    __IO uint32_t TCNT1;      /* Offset: 0x18, Timer counter 1 */
    __IO uint16_t TCR1;       /* Offset: 0x1C, Timer control register 1 */
    uint8_t       UNUSED3[2]; /* Offset: 0x1E */
    __IO uint32_t TCOR2;      /* Offset: 0x20, Timer constant register 2 */
    __IO uint32_t TCNT2;      /* Offset: 0x24, Timer counter 2 */
    __IO uint16_t TCR2;       /* Offset: 0x28, Timer control register 2 */
    __IO uint8_t  UNUSED4[2]; /* Offset: 0x2A */
    uint32_t      TCPR2;      /* Offset: 0x2C */
} TMU_TypeDef;

typedef struct {
    __IO uint32_t PTEH;        /* Offset: 0x00, Page table entry high register */
    __IO uint32_t PTEL;        /* Offset: 0x04, Page table entry low register */
    __IO uint32_t TTB;         /* Offset: 0x08, Translation table base register */
    __IO uint32_t TEA;         /* Offset: 0x0C, TLB exception address register */
    __IO uint32_t MMUCR;       /* Offset: 0x10, MMU control register */
    __IO uint8_t  BASRA;       /* Offset: 0x14, Break ASID register A*/
    uint8_t       UNUSED0[3];  /* Offset: 0x15 */
    __IO uint8_t  BASRB;       /* Offset: 0x18, Break ASID register B*/
    uint8_t       UNUSED1[3];  /* Offset: 0x19 */
    __IO uint32_t CCR;         /* Offset: 0x1C, Cache control register */
    __IO uint32_t TRA;         /* Offset: 0x20, TRAPA exception register */
    __IO uint32_t EXPEVT;      /* Offset: 0x24, Exception event register */
    __IO uint32_t INTEVT;      /* Offset: 0x28, Interrupt event register */
    uint32_t      UNUSED2[3];  /* Offset: 0x2C */
    __IO uint32_t QACR0;       /* Offset: 0x38, Queue address control register 0 */
    __IO uint32_t QACR1;       /* Offset: 0x3C, Queue address control register 1 */
    uint32_t      UNUSED3[12]; /* Offset: 0x40 */
    __IO uint32_t PASCR;       /* Offset: 0x70, Physical address space control register */
    __IO uint32_t RAMCR;       /* Offset: 0x74, On-chip memory control register */
    __IO uint32_t IRMCR;       /* Offset: 0x78, Instruction refetch inhibit control register */
} CSR_TypeDef;

#define XX sizeof(TMU_TypeDef)

#define PIO0_BASE (0xFD020000U)
#define PIO4_BASE (0xFD024000U)
#define ASC0_BASE (0xFD030000U)
#define ASC1_BASE (0xFD031000U)
#define ASC2_BASE (0xFD032000U)
#define ASC3_BASE (0xFD033000U)
#define CSR_BASE  (0xFF000000U)
#define TMU_BASE  (0xFFD80000U)

#define PIO0 ((PIO_TypeDef *)PIO0_BASE)
#define PIO4 ((PIO_TypeDef *)PIO4_BASE)
#define ASC0 ((ASC_TypeDef *)ASC0_BASE)
#define ASC1 ((ASC_TypeDef *)ASC1_BASE)
#define ASC2 ((ASC_TypeDef *)ASC2_BASE)
#define ASC3 ((ASC_TypeDef *)ASC3_BASE)
#define CSR  ((CSR_TypeDef *)CSR_BASE)
#define TMU  ((TMU_TypeDef *)TMU_BASE)

#endif