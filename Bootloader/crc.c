/*! \file crc.c
    \brief   CRC32 algorithm implementation
	
	\details CRC32 device driver. It leverages the CRC32 hardware of STM32F4XX devices.

	\date      2017
	\author    u-onder
	\copyright MIT License
*/

#include "crc.h"

//initialize and enable CRC hardware of device
int_t crcInit(void)
{
  //AHB1ENR: RCC AHB1 peripheral clock register, Address offset: 0x30
  *((i32_t*)(RCC_BASE + 0x00000030)) |= (ui32_t)0x00001000;
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_CRC, ENABLE);
  
  return SUCCESS;
}

//disable CRC hardware
int_t crcClose(void)
{
  //AHB1ENR: RCC AHB1 peripheral clock register, Address offset: 0x30
  *((i32_t*)(RCC_BASE + 0x00000030)) &= ~((ui32_t)0x00001000);
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_CRC, DISABLE);
  
  return SUCCESS;
}

//reset CRC hardware
int_t crcReset(void)
{
  *((i32_t*)(CRC_R_BASE + CRC_CR_OFFSET)) = (ui32_t)0x00000001;
  return SUCCESS;
}

//push to CRC harware and get result
ui32_t crcCalculate(ui32_t* buf, ui32_t len)
{
  ui32_t i = 0;
  
  for(i = 0; i < len; i++)
  {
    *((i32_t*)(CRC_R_BASE + CRC_DR_OFFSET)) = *buf;
    buf++;
  }
  
  return (*((i32_t*)(CRC_R_BASE + CRC_DR_OFFSET)));
}

//get latest CRC result
ui32_t crcGetCRC(void)
{
  return (*((i32_t*)(CRC_R_BASE + CRC_DR_OFFSET)));
}
