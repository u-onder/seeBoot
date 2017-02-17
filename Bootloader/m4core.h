#ifndef _M4CORE_H_
#define _M4CORE_H_

#include "cpu.h"

#define NVIC_R_BASE           (SCS_BASE +  0x0100UL)                    // NVIC Base Address
#define NVIC_ISER_OFFSET        ((ui32_t)0x0000)
#define NVIC_ICER_OFFSET        ((ui32_t)0x0080)
#define NVIC_ISPR_OFFSET        ((ui32_t)0x0100)
#define NVIC_ICPR_OFFSET        ((ui32_t)0x0180)
#define NVIC_IABR_OFFSET        ((ui32_t)0x0200)
#define NVIC_IPR_OFFSET         ((ui32_t)0x0300)
#define NVIC_STIR_OFFSET        ((ui32_t)0x0E00)

#define ITM_BASE            (0xE0000000UL)                            // ITM Base Address                 
#define CoreDebug_BASE      (0xE000EDF0UL)                            // Core Debug Base Address    

#define SCB_R_BASE            (SCS_BASE +  0x0D00UL)                    // System Control Block Base Address
#define SCB_CPUID_OFFSET          ((ui32_t)0x00)
#define SCB_ICSR_OFFSET           ((ui32_t)0x04)
#define SCB_VTOR_OFFSET           ((ui32_t)0x08)
#define SCB_AIRCR_OFFSET          ((ui32_t)0x0C)
#define SCB_SCR_OFFSET            ((ui32_t)0x10)
#define SCB_CCR_OFFSET            ((ui32_t)0x14)
#define SCB_SHP_OFFSET            ((ui32_t)0x18)
#define SCB_SHCSR_OFFSET          ((ui32_t)0x24)
#define SCB_CFSR_OFFSET           ((ui32_t)0x28)
#define SCB_HFSR_OFFSET           ((ui32_t)0x2C)
#define SCB_DFSR_OFFSET           ((ui32_t)0x30)
#define SCB_MMFAR_OFFSET          ((ui32_t)0x34)
#define SCB_BFAR_OFFSET           ((ui32_t)0x38)
#define SCB_AFSR_OFFSET           ((ui32_t)0x3C)
#define SCB_PFR_OFFSET            ((ui32_t)0x40)
#define SCB_DFR_OFFSET            ((ui32_t)0x48)
#define SCB_ADR_OFFSET            ((ui32_t)0x4C)
#define SCB_MMFR_OFFSET           ((ui32_t)0x50)
#define SCB_ISAR_OFFSET           ((ui32_t)0x60)
#define SCB_CPACR_OFFSET          ((ui32_t)0x88)

#define SYSTICK_R_BASE          (SCS_BASE +  0x0010UL)                    // SysTick Base Address             
#define SYSTICK_CTRL_OFFSET     ((ui32_t)0x00)
#define SYSTICK_LOAD_OFFSET     ((ui32_t)0x04)
#define SYSTICK_VAL_OFFSET      ((ui32_t)0x08)
#define SYSTICK_CALIB_OFFSET    ((ui32_t)0x0C)


#endif
