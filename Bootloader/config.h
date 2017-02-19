#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "cipher_config.h"
#include "serial_config.h"
#include "flash_config.h"

//! def PINCODE_LENGTH
//! brief Lenght of pin code of "break autoboot process".
//! brief This can be any N digit binary array, but must be consistent with PINCODE
#define PINCODE_LENGTH  10

//! def PINCODE
//! brief Pincode to break autoboot process, its length is PINCODE_LENGTH 
#define PINCODE "987654321"

//! brief Keys1 used in cipher and decipher processes.
//! warning seeBoot is a "Shared Secret" system, so these keys must be kept SECRET
#define KEY11     0x00112233  
#define KEY12     0x44556677
#define KEY13     0x8899AABB
#define KEY14     0xCCDDEEFF

//! brief Keys2 used in random IV cbc mode encryption.
//! warning seeBoot is a "Shared Secret" system, so these keys must be kept SECRET
#define KEY21     0x01234567  
#define KEY22     0x89ABCDEF
#define KEY23     0xFEDCBA98
#define KEY24     0x76543210

#endif
