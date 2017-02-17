#ifndef _CPU_H_
#define _CPU_H_

typedef unsigned char    ui08_t;
typedef   signed char    i08_t;
typedef unsigned short   ui16_t;
typedef   signed short   i16_t;
typedef unsigned int     ui32_t;
typedef   signed int     i32_t;
typedef unsigned long    ui64_t;
typedef   signed long    i64_t;

typedef unsigned char    uint8_t;
typedef   signed char    int8_t;

typedef unsigned int     uint_t;
typedef          int     int_t;
typedef         char     char_t;

typedef   signed int     handle_t;


 // defines 'write only' permissions
#define     __O     volatile            
// defines 'read / write' permissions
#define     __IO    volatile             

#define ERROR			(-1)
#define SUCCESS			(0)
#define INVALID_HANDLE	        (-1)

#define TRUE                    (1)
#define FALSE                   (0)

// Memory mapping of Cortex-M4 Hardware
#define SCS_BASE            (0xE000E000UL)                            // System Control Space Base Address

#if (__MPU_PRESENT == 1)
  #define MPU_BASE          (SCS_BASE +  0x0D90UL)                    // Memory Protection Unit             
#endif

#if (__FPU_PRESENT == 1)
  #define FPU_BASE          (SCS_BASE +  0x0F30UL)                    // Floating Point Unit                
#endif

// Peripheral_memory_map
#define CCMDATARAM_BASE       ((ui32_t)0x10000000) // CCM(core coupled memory) data RAM(64 KB) base address in the alias region 
#define SRAM1_BASE            ((ui32_t)0x20000000) // SRAM1(112 KB) base address in the alias region                            
#define SRAM2_BASE            ((ui32_t)0x2001C000) // SRAM2(16 KB) base address in the alias region                             
#define PERIPH_BASE           ((ui32_t)0x40000000) // Peripheral base address in the alias region                               
#define BKPSRAM_BASE          ((ui32_t)0x40024000) // Backup SRAM(4 KB) base address in the alias region                        
#define FSMC_R_BASE           ((ui32_t)0xA0000000) // FSMC registers base address                                               

#define CCMDATARAM_BB_BASE    ((ui32_t)0x12000000) // CCM(core coupled memory) data RAM(64 KB) base address in the bit-band region 
#define SRAM1_BB_BASE         ((ui32_t)0x22000000) // SRAM1(112 KB) base address in the bit-band region                            
#define SRAM2_BB_BASE         ((ui32_t)0x2201C000) // SRAM2(16 KB) base address in the bit-band region                             
#define PERIPH_BB_BASE        ((ui32_t)0x42000000) // Peripheral base address in the bit-band region                               
#define BKPSRAM_BB_BASE       ((ui32_t)0x42024000) // Backup SRAM(4 KB) base address in the bit-band region                        

// Legacy defines
#define SRAM_BASE             SRAM1_BASE
#define SRAM_BB_BASE          SRAM1_BB_BASE

// Peripheral memory map 
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x10000000)

/// APB1 peripherals
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400)
#define TIM12_BASE            (APB1PERIPH_BASE + 0x1800)
#define TIM13_BASE            (APB1PERIPH_BASE + 0x1C00)
#define TIM14_BASE            (APB1PERIPH_BASE + 0x2000)
#define RTC_BASE              (APB1PERIPH_BASE + 0x2800)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000)
#define I2S2ext_BASE          (APB1PERIPH_BASE + 0x3400)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00)
#define I2S3ext_BASE          (APB1PERIPH_BASE + 0x4000)
#define UART4_BASE            (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE            (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASE             (APB1PERIPH_BASE + 0x5C00)
#define CAN1_BASE             (APB1PERIPH_BASE + 0x6400)
#define CAN2_BASE             (APB1PERIPH_BASE + 0x6800)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE              (APB1PERIPH_BASE + 0x7400)

// APB2 peripherals
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x0400)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2000)
#define ADC2_BASE             (APB2PERIPH_BASE + 0x2100)
#define ADC3_BASE             (APB2PERIPH_BASE + 0x2200)
#define ADC_BASE              (APB2PERIPH_BASE + 0x2300)
#define SDIO_BASE             (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000)
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x3800)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x3C00)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800)

// AHB1 peripherals
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x3C00)
#define DMA1_BASE             (AHB1PERIPH_BASE + 0x6000)
#define DMA1_Stream0_BASE     (DMA1_BASE + 0x010)
#define DMA1_Stream1_BASE     (DMA1_BASE + 0x028)
#define DMA1_Stream2_BASE     (DMA1_BASE + 0x040)
#define DMA1_Stream3_BASE     (DMA1_BASE + 0x058)
#define DMA1_Stream4_BASE     (DMA1_BASE + 0x070)
#define DMA1_Stream5_BASE     (DMA1_BASE + 0x088)
#define DMA1_Stream6_BASE     (DMA1_BASE + 0x0A0)
#define DMA1_Stream7_BASE     (DMA1_BASE + 0x0B8)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x6400)
#define DMA2_Stream0_BASE     (DMA2_BASE + 0x010)
#define DMA2_Stream1_BASE     (DMA2_BASE + 0x028)
#define DMA2_Stream2_BASE     (DMA2_BASE + 0x040)
#define DMA2_Stream3_BASE     (DMA2_BASE + 0x058)
#define DMA2_Stream4_BASE     (DMA2_BASE + 0x070)
#define DMA2_Stream5_BASE     (DMA2_BASE + 0x088)
#define DMA2_Stream6_BASE     (DMA2_BASE + 0x0A0)
#define DMA2_Stream7_BASE     (DMA2_BASE + 0x0B8)
#define ETH_BASE              (AHB1PERIPH_BASE + 0x8000)
#define ETH_MAC_BASE          (ETH_BASE)
#define ETH_MMC_BASE          (ETH_BASE + 0x0100)
#define ETH_PTP_BASE          (ETH_BASE + 0x0700)
#define ETH_DMA_BASE          (ETH_BASE + 0x1000)

// AHB2 peripherals 
#define DCMI_BASE             (AHB2PERIPH_BASE + 0x50000)
#define CRYP_BASE             (AHB2PERIPH_BASE + 0x60000)
#define HASH_BASE             (AHB2PERIPH_BASE + 0x60400)
#define RNG_BASE              (AHB2PERIPH_BASE + 0x60800)

// FSMC Bankx registers base address
#define FSMC_Bank1_R_BASE     (FSMC_R_BASE + 0x0000)
#define FSMC_Bank1E_R_BASE    (FSMC_R_BASE + 0x0104)
#define FSMC_Bank2_R_BASE     (FSMC_R_BASE + 0x0060)
#define FSMC_Bank3_R_BASE     (FSMC_R_BASE + 0x0080)
#define FSMC_Bank4_R_BASE     (FSMC_R_BASE + 0x00A0)

// Debug MCU registers base address
#define DBGMCU_BASE           ((uint32_t )0xE0042000)

#endif
