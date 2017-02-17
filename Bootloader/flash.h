#ifndef _FLASH_H_
#define _FLASH_H_

#include "cpu.h"

#define FLASH_BASE              ((ui32_t)0x08000000) // FLASH(up to 1 MB) base address in the alias region                        

#define FLASH_R_BASE            (AHB1PERIPH_BASE + 0x3C00)
#define FLASH_ACR_OFFSET        ((ui32_t)0x00)
#define FLASH_KEYR_OFFSET       ((ui32_t)0x04)
#define FLASH_OPTKEYR_OFFSET    ((ui32_t)0x08)
#define FLASH_SR_OFFSET         ((ui32_t)0x0C)
#define FLASH_CR_OFFSET         ((ui32_t)0x10)
#define FLASH_OPTCR_OFFSET      ((ui32_t)0x14)

int_t flashInit          (void);
int_t flashClose         (void);

int_t flashGetNameString (char_t* dst);
int_t flashErease        (void);
int_t flashWriteAddress  (ui32_t* buf, ui32_t address, uint_t len);
int_t flashVerifyAddress (ui32_t* src, ui32_t address, uint_t len);

#endif
