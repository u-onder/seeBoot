#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "cpu.h"

#define USART1_BASE           (APB2PERIPH_BASE + 0x1000)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800)
#define USART6_BASE           (APB2PERIPH_BASE + 0x1400)

#define USART_SR_OFFSET         ((ui32_t)0x00)
#define USART_DR_OFFSET         ((ui32_t)0x04)
#define USART_BRR_OFFSET        ((ui32_t)0x08)
#define USART_CR1_OFFSET        ((ui32_t)0x0C)
#define USART_CR2_OFFSET        ((ui32_t)0x10)
#define USART_CR3_OFFSET        ((ui32_t)0x14)
#define USART_GTPR_OFFSET       ((ui32_t)0x18)

int_t    serialInit (void);
int_t    serialOpen (void);
int_t    serialClose(void);
int_t    serialRead (char_t* byte);
int_t    serialWriteStr(char_t* str);
int_t    serialWriteBin(char_t* str, int_t ln);
int_t    serialFlush(void);

#endif
