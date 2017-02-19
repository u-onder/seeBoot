/*! \file    comm.c

    \brief   Communication Protocol

    \details Serial communication protocol source file.

	\warning Serial comm protocol only supports upper case hexedecimal values, since 
	         lower case characters are reserved for command characters
	
	\date      2017
	\author    u-onder
	\copyright MIT License
*/

#include "config.h"

#include "comm.h"
#include "serial.h"
#include "misc.h"

//! \brief local scope variable for checksum calculation
static char_t commChecksum;

//! \brief variable, storing one single string
static char_t  commStr[256];
//! \brief string index pointer of comm parser
static int_t   commStrIndex;
//! \brief comm string binary checksum value, used while transmit
static char_t  commStrChecksumVal; 
//! \brief comm strings readable checksum value
static char_t  commStrChecksum[2];
//! \brief checksum string index pointer of comm parser
static int_t   commStrChecksumIndex;

//! \brief   Initialization of comm parser
//! \return \c SUCCESS or \c ERROR
int_t commInit(void)
{
  //init serial channel
  serialInit();
  serialOpen();
  
  //init comm specific definitions
  commStrIndex = -1;
  commStrChecksumIndex = -1;
  
  return SUCCESS;
}

//! \brief   Destructor of comm parser
//! \return \c SUCCESS or \c ERROR
int_t commDispose(void)
{
  //close serial channel
  serialClose();
  
  return SUCCESS;
}


//! \brief   send comm string header if any
//! \return \c SUCCESS or \c ERROR
//! \todo NULL
//! \note NULL
//! \sa commSendBody(), commSendFooter()
int_t commSendHeader (void)
{
  serialWriteStr("c");
  commChecksum = 0x00;

  return SUCCESS;
}

//! \brief   Send a bunch of characters as body of a comm string
//! \param[in] str (partial) comm body as string 
//! \return \c SUCCESS or \c ERROR
//! \note str must be a c style \c null terminated string
//! \sa commSendHeader(), commSendFooter()
int_t commSendBody (char_t* str)
{
  while (*str)
  {
    commChecksum ^= *str;
    serialWriteBin((char_t*)str, 1);
    str++;
  }
  return SUCCESS;
}

//! \brief   Send a comm footer, if any
//! \param[in] sendCRC append CRC?
//! \return \c SUCCESS or \c ERROR
//! \sa commSendHeader(), commSendBody()
int_t commSendFooter (int_t sendCRC)
{
  int8_t HB, LB;
  char_t dummy;

  if(sendCRC != 0)
  {
	  serialWriteStr(".");
	  
	  HB = commChecksum >> 4;
	  LB = commChecksum & 0x0F;
	  
	  dummy = IntToHex(HB);
	  serialWriteBin(&dummy, 1);
	  dummy = IntToHex(LB);
	  serialWriteBin(&dummy, 1);
  }  
  serialWriteStr("\r\n");
  
  return SUCCESS;
}

int_t commFlush (void)
{
  serialFlush();

  return SUCCESS;
}


//! \brief   parse comm strings from input stream
//! \return \c NULL or comm command string
//! \todo NULL
//! \note returned string is not dynamic, a call to commParse() will most probably
//!       corrupt the comm string. If one have to call this function while still
//!       using the returned string, she must consider of backup to seperate buffer
char_t* commParse (void)
{
  char_t c;
  int8_t HB, LB;
  
  char_t* ret = (char_t*) 0;
  
  while (serialRead(&c) == SUCCESS)
  {
    switch (c)
    {
      case 'c':
        commStrIndex = 0;
        commStrChecksumIndex = -1;
        commStrChecksumVal = 0;
        break;

        
      case '.':
        commStr[commStrIndex] = '\0';   //null terminate body string
        commStrChecksumIndex = 0;       //get checksum
        break;
        
      case '\r':
      case '\n':
        if ((commStrIndex  > 0) && (commStrChecksumIndex == 2))
        {
          HB = commStrChecksumVal >> 4;
          LB = commStrChecksumVal & 0x0F;
          
          if ((HexToInt(commStrChecksum[0]) == HB) && (HexToInt(commStrChecksum[1]) == LB))
            ret = commStr;
        }
        commStrIndex = -1;
        commStrChecksumIndex = -1;            
        
        if (ret != (char_t*)0)
          return ret;
        
        break;
        
      default:
        if (commStrIndex < 0)
          break;
        
        if ((commStrChecksumIndex >= 0) && (commStrChecksumIndex < 2))
        {
          commStrChecksum[commStrChecksumIndex] = c;
          commStrChecksumIndex++;
        }
        else if (commStrChecksumIndex < 0)
        {
          if (commStrIndex < 255)
          {
            commStr[commStrIndex] = c;
            commStrIndex++;
            commStrChecksumVal ^= c;
          }
        }
        
        break;
    }
  }

  return ret;
}
