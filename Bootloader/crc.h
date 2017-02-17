#ifndef _CRC_H_
#define _CRC_H_

#include "cpu.h"

#define CRC_R_BASE              (AHB1PERIPH_BASE + 0x3000)
#define CRC_DR_OFFSET           ((ui32_t)0x00)
#define CRC_IDR_OFFSET          ((ui32_t)0x04)
#define CRC_CR_OFFSET           ((ui32_t)0x08)

int_t  crcInit      (void);
int_t  crcClose     (void);

int_t  crcReset     (void);
ui32_t crcCalculate (ui32_t* buf, ui32_t len);
ui32_t crcGetCRC    (void);

#endif
