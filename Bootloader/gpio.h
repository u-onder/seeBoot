#ifndef _GPIO_H_
#define _GPIO_H_

#include "cpu.h"

#define GPIOA_R_BASE            (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_R_BASE            (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_R_BASE            (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_R_BASE            (AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_R_BASE            (AHB1PERIPH_BASE + 0x1000)
#define GPIOF_R_BASE            (AHB1PERIPH_BASE + 0x1400)
#define GPIOG_R_BASE            (AHB1PERIPH_BASE + 0x1800)
#define GPIOH_R_BASE            (AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_R_BASE            (AHB1PERIPH_BASE + 0x2000)

#define GPIO_MODER_OFFSET       ((ui32_t)0x00)
#define GPIO_OTYPER_OFFSET 	((ui32_t)0x04)
#define GPIO_OSPEEDR_OFFSET	((ui32_t)0x08)
#define GPIO_PUPDR_OFFSET  	((ui32_t)0x0C)
#define GPIO_IDR_OFFSET    	((ui32_t)0x10)
#define GPIO_ODR_OFFSET    	((ui32_t)0x14)
#define GPIO_BSRR_OFFSET   	((ui32_t)0x18)
#define GPIO_LCKR_OFFSET   	((ui32_t)0x1C)
#define GPIO_AFRL_OFFSET   	((ui32_t)0x20)
#define GPIO_AFRH_OFFSET   	((ui32_t)0x24)

#endif
