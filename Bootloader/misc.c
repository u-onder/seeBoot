/*! \file misc.c
    \brief   miscellaneous functions
	
	\date      2017
	\author    u-onder
	\copyright MIT License
*/

#include "misc.h"

char_t IntToHex(int_t x)
{
  return (x > 9 ? 'A' + x - 10 : '0' + x);
}

int_t HexToInt(char_t x)
{
  if ((x >= '0') && (x <= '9'))
    return x - '0';
  
  if ((x >= 'a') && (x <= 'f'))
    return x - 'a' + 10;
  
  if ((x >= 'A') && (x <= 'F'))
    return x - 'A' + 10;
  
  return 0;
}
