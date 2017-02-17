#include "..\\cpu.h"
#include "..\\m4core.h"
#include "..\\flash.h"
 /*
  *=============================================================================
  *        Supported STM32F4xx device revision    | Rev A
  *-----------------------------------------------------------------------------
  *        System Clock source                    | PLL (HSE)
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                             | 168000000
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                               | 168000000
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                         | 4
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                         | 2
  *-----------------------------------------------------------------------------
  *        HSE Frequency(Hz)                      | 16000000
  *-----------------------------------------------------------------------------
  *        PLL_M                                  | 16
  *-----------------------------------------------------------------------------
  *        PLL_N                                  | 336
  *-----------------------------------------------------------------------------
  *        PLL_P                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL_Q                                  | 7
  *-----------------------------------------------------------------------------
  *        PLLI2S_N                               | NA
  *-----------------------------------------------------------------------------
  *        PLLI2S_R                               | NA
  *-----------------------------------------------------------------------------
  *        I2S input clock                        | NA
  *-----------------------------------------------------------------------------
  *        VDD(V)                                 | 3.3
  *-----------------------------------------------------------------------------
  *        Main regulator output voltage          | Scale1 mode
  *-----------------------------------------------------------------------------
  *        Flash Latency(WS)                      | 5
  *-----------------------------------------------------------------------------
  *        Prefetch Buffer                        | OFF
  *-----------------------------------------------------------------------------
  *        Instruction cache                      | ON
  *-----------------------------------------------------------------------------
  *        Data cache                             | ON
  *-----------------------------------------------------------------------------
  *        Require 48MHz for USB OTG FS,          | Disabled
  *        SDIO and RNG clock                     |
  *-----------------------------------------------------------------------------
  *=============================================================================
  */

void SetSysClock(void);

/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemFrequency variable.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  // Reset the RCC clock configuration to the default reset state ------------
  //SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
  
  // Set HSION bit 
  //CR: RCC clock control register, Address offset: 0x00
  *((i32_t*)(RCC_BASE + 0x00000000)) |= (ui32_t)0x00000001;
  //RCC->CR |= (uint32_t)0x00000001;

  // Reset CFGR register 
  //CFGR: RCC clock configuration register, Address offset: 0x08
  *((i32_t*)(RCC_BASE + 0x00000008)) = 0x00000000;
  //RCC->CFGR = 0x00000000;

  // Reset HSEON, CSSON and PLLON bits
  //CR: RCC clock control register, Address offset: 0x00
   *((i32_t*)(RCC_BASE + 0x00000000)) &= (ui32_t)0xFEF6FFFF;
  //RCC->CR &= (uint32_t)0xFEF6FFFF;

  // Reset PLLCFGR register
   //PLLCFGR: RCC PLL configuration register, Address offset: 0x04
  *((i32_t*)(RCC_BASE + 0x00000004)) = (ui32_t)0x24003010;
  //RCC->PLLCFGR = 0x24003010;

  // Reset HSEBYP bit
  //CR: RCC clock control register, Address offset: 0x00
  *((i32_t*)(RCC_BASE + 0x00000000))&= (ui32_t)0xFFFBFFFF;
  //RCC->CR &= (uint32_t)0xFFFBFFFF;

  // Disable all interrupts
  //CIR: RCC clock interrupt register, Address offset: 0x0C 
  *((i32_t*)(RCC_BASE + 0x0000000C)) = (ui32_t)0x00000000;
  //RCC->CIR = 0x00000000;

  // Configure the System clock source, PLL Multiplier and Divider factors, 
  // AHB/APBx prescalers and Flash settings ----------------------------------
  SetSysClock();

  // Configure the Vector Table location add offset address ------------------
  *((ui64_t*)(SCB_R_BASE + SCB_VTOR_OFFSET)) = FLASH_BASE;
  //SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
}

#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((ui32_t)25000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */
#if !defined  (HSE_STARTUP_TIMEOUT) 
  #define HSE_STARTUP_TIMEOUT    ((ui16_t)0x0500)   /*!< Time out for HSE start up */
#endif /* HSE_STARTUP_TIMEOUT */  

/**
  * @brief  Configures the System clock source, PLL Multiplier and Divider factors, 
  *         AHB/APBx prescalers and Flash settings
  * @Note   This function should be called only once the RCC clock configuration  
  *         is reset to the default reset state (done in SystemInit() function).   
  * @param  None
  * @retval None
  */
void SetSysClock(void)
{
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
  __IO ui32_t StartUpCounter = 0, HSEStatus = 0;
  
  // Enable HSE
  //CR: RCC clock control register, Address offset: 0x00
   *((i32_t*)(RCC_BASE + 0x00000000)) |= ((ui32_t)0x00010000);
  //RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  // Wait till HSE is ready and if Time out is reached exit
  do
  {
    //CR: RCC clock control register, Address offset: 0x00
    HSEStatus = (*((i32_t*)(RCC_BASE + 0x00000000))) & ((ui32_t)0x00020000);
    //HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  //CR: RCC clock control register, Address offset: 0x00
   if (((*((i32_t*)(RCC_BASE + 0x00000000))) & ((ui32_t)0x00020000)) != 0)
  //if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (ui32_t)0x01;
  }
  else
  {
    HSEStatus = (ui32_t)0x00;
  }

  if (HSEStatus == (ui32_t)0x01)
  {
    // Enable high performance mode, System frequency up to 168 MHz
    //APB1ENR: RCC APB1 peripheral clock enable register, Address offset: 0x40
   *((i32_t*)(RCC_BASE + 0x00000040)) |= ((ui32_t)0x10000000);
    //RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    //CR: RCC clock control register, Address offset: 0x00
    *((i32_t*)(PWR_BASE + 0x00000000)) |= ((ui16_t)0x4000);
    //PWR->CR |= PWR_CR_PMODE;  

    // HCLK = SYSCLK / 1
    //CFGR: RCC clock configuration register, Address offset: 0x08
    *((i32_t*)(RCC_BASE + 0x00000008)) |= ((ui32_t)0x00000000);
    //RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
      
    // PCLK2 = HCLK / 2
    //CFGR: RCC clock configuration register, Address offset: 0x08
    *((i32_t*)(RCC_BASE + 0x00000008)) |= ((ui32_t)0x00008000);
    //RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    // PCLK1 = HCLK / 4
    //CFGR: RCC clock configuration register, Address offset: 0x08
    *((i32_t*)(RCC_BASE + 0x00000008)) |= ((ui32_t)0x00001400);
    //RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

#define PLL_M      16
#define PLL_N      336
#define PLL_P      2
#define PLL_Q      7
    
    // Configure the main PLL
    //PLLCFGR: RCC PLL configuration register, Address offset: 0x04
    *((i32_t*)(RCC_BASE + 0x00000004)) = PLL_M | 
                                         (PLL_N << 6) | 
                                         (((PLL_P >> 1) -1) << 16) | 
                                         (((ui32_t)0x00400000)) | 
                                         (PLL_Q << 24);
    //RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
    
    // Enable the main PLL
    //CR: RCC clock control register, Address offset: 0x00
    *((i32_t*)(RCC_BASE + 0x00000000)) |= ((ui32_t)0x01000000);
    //RCC->CR |= RCC_CR_PLLON;

    // Wait till the main PLL is ready 
    while(((*((i32_t*)(RCC_BASE + 0x00000000))) & ((ui32_t)0x02000000)) == 0);
    // while((RCC->CR & RCC_CR_PLLRDY) == 0);
   
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    //ACR: FLASH access control register, Address offset: 0x00
    *((ui32_t*)(FLASH_R_BASE + 0x00000000)) = ((ui32_t)0x00000200) | ((ui32_t)0x00000400) | ((ui32_t)0x00000005);
    //FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    // Select the main PLL as system clock source 
    //CFGR: RCC clock configuration register, Address offset: 0x08
    *((i32_t*)(RCC_BASE + 0x00000008)) &= (ui32_t)((ui32_t)~(((ui32_t)0x00000003)));
    *((i32_t*)(RCC_BASE + 0x00000008)) |= ((ui32_t)0x00000002);
    //RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    //RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while (((*((i32_t*)(RCC_BASE + 0x00000008))) & ((ui32_t)0x0000000C) ) != ((ui32_t)0x00000008));
    //while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }

}
