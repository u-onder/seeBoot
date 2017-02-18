/*! \file timer.c
    \brief   System timer handler
	
	\details This is the timer driver of STM32f4 sys timer.
	\details The resolution of the timer driver is 8 ms.

	\date      2017
	\author    u-onder
	\copyright MIT License
*/

#include "timer.h"

#include "m4core.h"

ui32_t timerTimeout;

//timer initialize and enable
int_t  timerInit  (void)
{
  //interrupt for every 1 ms.
  *((i32_t*)(SYSTICK_R_BASE + SYSTICK_LOAD_OFFSET)) = 1344000;
   ((i32_t*)(SCB_R_BASE + SCB_SHP_OFFSET))[3] |= 0xF0000000;
  *((i32_t*)(SYSTICK_R_BASE + SYSTICK_VAL_OFFSET))  = 0;    
  *((i32_t*)(SYSTICK_R_BASE + SYSTICK_CTRL_OFFSET)) = 0x00000007;  
  
  return SUCCESS;
}

//disable timer
int_t  timerClose (void)
{
  *((i32_t*)(SYSTICK_R_BASE + SYSTICK_CTRL_OFFSET)) = 0x00000000;
  
  return SUCCESS;
}

//set a timeout
void   timerSet   (ui32_t timeout)
{
  *((i32_t*)(SYSTICK_R_BASE + SYSTICK_VAL_OFFSET))  = 0;
  timerTimeout = ((timeout+7)>>3);
}

//get timeout
ui32_t timerGet   (void)
{
  return (timerTimeout<<3);
}

//timer ISR callback
void SysTick_Handler(void)
{
  if (timerTimeout)
    timerTimeout--;
}
