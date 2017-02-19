/*! \file flash.c
    \brief   seeBoot flash dependent algorithms and drivers
	
        \details Flash dependent read, write operation handler. Driver has following features:
        \details Erease, Write, Verify, Get Image Name String...
        \details This flash handle implementation unlocks flash write protection flags.
                 So user must call close handler before any boot operation...

	\date      2017
	\author    u-onder
	\copyright MIT License
*/

#include "config.h"
#include "flash.h"
#include "crc.h"

//local scope functions
int_t __flashRead   (ui32_t* dst, ui32_t address, uint_t len);
int_t __flashReadStr(char_t* dst, ui32_t address, uint_t len);

typedef struct {
  ui32_t startAddress;
  ui32_t endAddress;
}sectorDefs_t;

sectorDefs_t sectorDefs[12] = {
  {0x08000000, 0x08003FFF},
  {0x08004000, 0x08007FFF},
  {0x08008000, 0x0800BFFF},
  {0x0800C000, 0x0800FFFF},
  {0x08010000, 0x0801FFFF},
  {0x08020000, 0x0803FFFF},
  {0x08040000, 0x0805FFFF},
  {0x08060000, 0x0807FFFF},
  {0x08080000, 0x0809FFFF},
  {0x080A0000, 0x080BFFFF},
  {0x080C0000, 0x080DFFFF},
  {0x080E0000, 0x080FFFFF},
};

//initialize flash for write operations
int_t flashInit (void)
{
  crcInit();

  //unlock flash control registers
  if((*((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET))) & 0x80000000)
  {
    *((i32_t*)(FLASH_R_BASE + FLASH_KEYR_OFFSET)) = ((i32_t)0x45670123);  //Key1
    *((i32_t*)(FLASH_R_BASE + FLASH_KEYR_OFFSET)) = ((i32_t)0xCDEF89AB);  //Key2
  }  
  
  *((i32_t*)(FLASH_R_BASE + FLASH_SR_OFFSET)) |= 0x000000F2;    //clear any previous error
  return SUCCESS;
}

//disable flash write
int_t flashClose (void)
{
  crcClose();

  //lock flash control registers
  *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) |= 0x80000000;
  
  return SUCCESS;
}

//read name string from flash
int_t flashGetNameString (char_t* dst)
{
  ui32_t crc1, crc2;
    
  crcReset();
  crc1 = crcCalculate((ui32_t*)BOOTIMAGE_START_ADDRESS, (ui32_t)(BOOTIMAGE_HASH_ADDRESS - BOOTIMAGE_START_ADDRESS)/4);
  __flashRead(&crc2, BOOTIMAGE_HASH_ADDRESS, 1);
  if (crc1 != crc2)
    return ERROR;
  
  __flashReadStr(dst, BOOTIMAGE_SIGNATURE_ADDRESS, BOOTIMAGE_NAME_SIZE);
  dst[BOOTIMAGE_NAME_SIZE] = '\0';
  
  return SUCCESS;
}

//ereae flash
int_t flashErease (void)
{
  ui32_t status;
  ui32_t sector;
  ui32_t i;
  int_t  isEmpty;
  ui32_t dummy;
  
  sector = 1;   //skip sector 0 since it is bootloader sector
  while (sector < 12)
  {
    isEmpty = 1;
    for (i = sectorDefs[sector].startAddress; i < sectorDefs[sector].endAddress; i+=4)
    {
      __flashRead(&dummy, i, 1);
      if (dummy != 0xFF)
      {
        isEmpty = 0;
        break;
      }
    }
    
    if (isEmpty == 1)
    {
      sector++;
      continue;
    }
    
    //clear pending flash access flags
    *((i32_t*)(FLASH_R_BASE + FLASH_SR_OFFSET)) = 0x000000F3;

    //wait flash to complete its last operation successfuly
    do { 
      status = *((i32_t*)(FLASH_R_BASE + FLASH_SR_OFFSET));
    } while (status & 0x00010000);
    
    if (status & 0x000000F2)
      continue;
    
    //process erease operation
    *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) &= ~((ui32_t)0x00000300);
    *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) |=  ((ui32_t)0x00000200);
    *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) &= ~((ui32_t)0x00000078);
    *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) |=  ((ui32_t)sector << 3);
    *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) |=  ((ui32_t)0x00000002);
    *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) |=  ((ui32_t)0x00010000);
    
    // Wait for last operation to be completed
    do { 
      status = *((i32_t*)(FLASH_R_BASE + FLASH_SR_OFFSET));
    } while (status & 0x00010000);
    
    // if the erase operation is completed, disable the SER Bit
    *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) &= ~((ui32_t)0x00000002);
    
    sector++;
  }

  return ERROR;
}

//write to address
int_t flashWriteAddress  (ui32_t* buf, ui32_t address, uint_t len)
{
  ui32_t status;
  
  if (address >= BOOTIMAGE_HASH_ADDRESS + 16)
    return ERROR;
  if (address < BOOTIMAGE_START_ADDRESS)
    return ERROR;
  
  while (len)
  {   
    //wait flash to complete its last operation successfuly
    do { 
      status = *((i32_t*)(FLASH_R_BASE + FLASH_SR_OFFSET));
    } while (status & 0x00010000);
      
    if (status & 0x000000F2)
      return ERROR;

      //process write operation
      *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) &= ~((ui32_t)0x00000300);
      *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) |=  ((ui32_t)0x00000200);
      *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) |=  ((ui32_t)0x00000001);
    
      *(__IO ui32_t*)address = *buf;
          
    //wait flash to complete its last operation successfuly
    do { 
      status = *((i32_t*)(FLASH_R_BASE + FLASH_SR_OFFSET));
    } while (status & 0x00010000);

      *((i32_t*)(FLASH_R_BASE + FLASH_CR_OFFSET)) &= ~((ui32_t)0x00000001);
    
    len--;
    buf++;
    address +=4;
  }

  return SUCCESS;
}

//verify from address
int_t flashVerifyAddress (ui32_t* src, ui32_t address, uint_t len)
{
  if (address >= BOOTIMAGE_HASH_ADDRESS + 16)
    return ERROR;
  if (address < BOOTIMAGE_START_ADDRESS)
    return ERROR;
  
  while (len)
  {       
    if (*(ui32_t*)address != *src)
      return ERROR;
          
    len--;
    src++;
    address +=4;
  }

  return SUCCESS;
}

//(local call only) read flash
int_t __flashRead (ui32_t* dst, ui32_t address, uint_t len)
{  
  while (len)
  {
    *dst = *(ui32_t*)(address);
    dst++;
    address += 4;
    len--;
  }
  
  return ERROR;
}

//(local call only) read null terminated string from flash
int_t __flashReadStr (char_t* dst, ui32_t address, uint_t len)
{
  char_t* pch = (char_t*)(address);
  
  while (len)
  {
    *dst++ = *pch++;
    len--;
  }
  
  return ERROR;
}

