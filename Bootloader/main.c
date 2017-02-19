/*! \file main.c
    \brief   Bootloader main algorithm implementation
	
	\details Bootloaders main algorithm is implemented here.
	\details After power up (HW or SW reset), bootloader will check for a valid signed 
             boot image. If it founds one, it will inform host with image name string, and
             initialize an auto-boot sequence. But if it fails to find a proper boot image
             it will print a 'boot image not found' message and fall into boot command shell
	\details The auto-boot procedure can be interrupted. seeBoot will
             fall in to boot command shell. If no boot image is found bootloader will
             automatically  shell.
        \details Shell commands:
        \details b: (cb.62<CR><LF>) immediate boot.
        \details e: (ce.65<CR><LF>) full chip erease. This may take several secondes 
        \details c: (cp<DATA>.<CRC><CR><LF>) program block with from cipher text.
        \details v: (cv<DATA>.<CRC><CR><LF>) verify block against given cipher text.
        \details i: (ci<PIN>.<CRC><CR><LF>) auto-boot interrupt pin

        \details Shell responces:
        \details a: (ca.61<CR><LF>) Acknowledgement
        \details n: (cn.6E<CR><LF>) Not-Acknowledge

	\warning Serial comm protocol only supports upper case hexedecimal values, since 
	         lower case characters are reserved for command characters
		
	\date      2017
	\author    u-onder
	\copyright MIT License
*/

#include "config.h"

#include "comm.h"
#include "flash.h"
#include "timer.h"
#include "cipher.h"
#include "crc.h"

#include "m4core.h"

#include "misc.h"

#include <string.h>

//! \brief command character
static char_t  cmdChar;
//! \brief command argument string
static char_t  *cmdArgStr;

//! \brief argument count of parsed command
static  int_t   cmdArgCnt;


typedef struct{
  i32_t  iv[2];
  i32_t  address;
  i32_t  data[16];
  i32_t  crc; 
} bootProgramPacket_t;

void    __sendAck          (void);
void    __sendNack         (void);

char_t* __parseCommCommand (void);
int_t   __waitPINCommand   (void);
int_t   __waitBootCommand  (void);

void    __boot             (ui32_t address);

//
// BOOT PROCEDURE NMEAS
//-----------------------------------------------------------------------------
//bootloader name string
char_t BootloaderNameString[]    = "seeBoot v0.1";

int_t  bootable = ERROR;
char_t bootImageName[BOOTIMAGE_NAME_SIZE+1];
int_t  bootTimeout;

i08_t  checksum;

i08_t  pin[PINCODE_LENGTH] = PINCODE;
int_t  pinIndex;

i32_t key1[4] = {KEY11, KEY12, KEY13, KEY14};
i32_t key2[4] = {KEY21, KEY22, KEY23, KEY24};

bootProgramPacket_t pkt;
ui32_t tmp[32];

void main (void)
{ 
  ui32_t crc;
  
  //0 - initialize timer
  timerInit();

  //comm init
  commInit();

  //flash initialize 
  flashInit();

  timerSet(1000);      // 1 seconds timeout
  while (timerGet());
  
  for (;;)
  {
    // print bootloader name string (bootloader name, version etc)
    commSendHeader();
    commSendBody(" ");
    commSendBody(BootloaderNameString);
    commSendFooter(0);
    // print a bootloader self test string
	commSendHeader();
	commSendBody(" Cheking Firmware...");
	commSendFooter(0);
    
    //1 - check for a valid, digitally signed code
    bootable = flashGetNameString(bootImageName);
    
    if (bootable == SUCCESS)
    {
      // image found!
      // wait for a valid pin to break auto-boot
      
      // print boot image name string
      commSendHeader();
      commSendBody(" Firmware Found: ");
      commSendBody(bootImageName);
      commSendFooter(0);
      
      //auto boot in 5 seconds
      bootTimeout = 5;
      
      //wait for pin...
      do
      {
        char_t dummy[2] ="x";

        //print boot in X seconds string
        commSendHeader();
        commSendBody(" Auto-boot in ");
        dummy[0] = '0' + bootTimeout;
        commSendBody(dummy);
        commSendFooter(0);

        //note: waitPINCommand is a blocking function!
        if (__waitPINCommand() == SUCCESS)
          break;
        
        bootTimeout--;
      }while (bootTimeout > 0);
    }
    else //if not bootable
    {
      //print no auto-boot string. ie boot in -1 seconds string
      bootTimeout = -1;
      
	  commSendHeader();
      commSendBody(" No bootable image found!");
      commSendFooter(0);
    }

    if (bootTimeout > 0 )
    {
      // INTERRUPTED / MANUAL BOOT...
    }
    else if (bootTimeout == 0)
    {
      // AUTO BOOT...
	  commSendHeader();
	  commSendBody(" Booting...");
	  commSendFooter(0);
      commFlush();
      __boot(BOOTIMAGE_START_ADDRESS);
    }
    else // -1
    {
      // NO BOOT IMAGE!...
    }

    // inform server about manual boot
	commSendHeader();
    commSendBody(" Manual Boot (Boot Command Shell)");
    commSendFooter(0);
    
    timerSet(60000);      // 60 seconds timeout
    while (timerGet())
    {
      if (__waitBootCommand() == ERROR)
        continue;

      timerSet(60000);
      
      //boot-now command
      if (cmdChar == 'b')
      {
        bootable = flashGetNameString(bootImageName);
        
        if (bootable == SUCCESS)
          __sendAck();
        else
          __sendNack();
        
        commFlush();
        
        if (bootable == SUCCESS)
          __boot(BOOTIMAGE_START_ADDRESS);
        continue;
      }
            
      //erase command
      if (cmdChar == 'e')
      {
        flashErease();
        __sendAck();
        continue;
      }
      
      //program command
      if (cmdChar == 'p')
      {
        int_t i;
        
        if ((cmdArgCnt != 1) || (strlen(cmdArgStr) != sizeof(pkt)*2))
        {
          __sendNack();
          continue;
        }
        //fill packet structure
        for (i = 0; i < sizeof(pkt); i++)
          ((char_t*)&pkt)[i] = ((HexToInt(cmdArgStr[i*2+1])) | (HexToInt(cmdArgStr[i*2]) << 4));
        
        blockDecipher((ui32_t*)pkt.iv, (ui32_t*)key1, (ui32_t*)key2, (ui32_t*)&pkt.address, (sizeof(pkt) - sizeof(pkt.iv)) / sizeof(ui32_t));
        
        crcReset();
        crc = crcCalculate((ui32_t*)pkt.iv, (sizeof(pkt) - sizeof(pkt.crc)) / sizeof(ui32_t));
        if (pkt.crc != crc)
        {
          __sendNack();
          continue;
        }

        if (flashWriteAddress((ui32_t*)pkt.data, pkt.address, sizeof(pkt.data) / sizeof(ui32_t)) == SUCCESS)
          __sendAck();
        else
          __sendNack();
        
        continue;
      }
      
      //verify
      if (cmdChar == 'v')
      {
        int_t i;
        
        if ((cmdArgCnt != 1) || (strlen(cmdArgStr) != sizeof(pkt)*2))
        {
          __sendNack();
          continue;
        }
        //fill packet structure
        for (i = 0; i < sizeof(pkt); i++)
          ((char_t*)&pkt)[i] = ((HexToInt(cmdArgStr[i*2+1])) | (HexToInt(cmdArgStr[i*2]) << 4));
                    
        blockDecipher((ui32_t*)pkt.iv, (ui32_t*)key1, (ui32_t*)key2, (ui32_t*)&pkt.address, (sizeof(pkt) - sizeof(pkt.iv)) / sizeof(ui32_t));
        
        crcReset();
        crc = crcCalculate((ui32_t*)pkt.iv, (sizeof(pkt) - sizeof(pkt.crc)) / sizeof(ui32_t));
        if (pkt.crc != crc)
        {
          __sendNack();
          continue;
        }

        if (flashVerifyAddress((ui32_t*)pkt.data, pkt.address, sizeof(pkt.data) / sizeof(ui32_t)) == SUCCESS)
          __sendAck();
        else
          __sendNack();
        continue;      
      }
    }
  }
}

void __boot(ui32_t address)
{
  const ui32_t *app_base = (const ui32_t *)address;
  ui32_t stacktop   = app_base[0];
  ui32_t entrypoint = app_base[1];
  
  //close all peripherals
  timerClose();
  commDispose();
  flashClose();

  // switch exception handlers to the application
  *((i32_t*)(SCB_R_BASE + SCB_VTOR_OFFSET)) = address;

  // extract the stack and entrypoint from the app vector table and go
  asm volatile(
          "msr msp, %0	\n"
          "bx	%1	\n"
          : : "r" (stacktop), "r" (entrypoint) : );
  for (;;);
}

void __sendAck(void)
{
    commSendHeader();
    commSendBody("a");
    commSendFooter(1);
}

void __sendNack(void)
{
    commSendHeader();
    commSendBody("n");
    commSendFooter(1);
}

int_t __waitPINCommand(void)
{
  timerSet(1000);
  while (timerGet())
  {
    if (__waitBootCommand() == ERROR)
      continue;

    //pin command is something like this
    //ci<Pin String>

    //check for a valid command: PIN
    if (cmdChar != 'i')
    {
      __sendNack();
      continue;
    }
    //check for a valid pin
    if ((cmdArgCnt != 1) || (strcmp((char const*)cmdArgStr,PINCODE) != 0))
    {
      //pin is invalid
      __sendNack();
      continue;
    }
    
    __sendAck();
    return SUCCESS;
  }  
  return ERROR;
}

int_t __waitBootCommand(void)
{
  char_t* cmdStr;

  cmdArgCnt = -1;
  cmdArgStr = (char_t*) 0;
  
  //parse command
  cmdStr = commParse();
  if (cmdStr == (char_t *)0)
    return ERROR;
  
  cmdArgCnt = 0;
  cmdChar = *cmdStr;
  cmdStr++;
  
  if (*cmdStr)
  {
	cmdArgStr = cmdStr;
	cmdArgCnt = 1;
	return SUCCESS;
  }
  
  return SUCCESS;
}
