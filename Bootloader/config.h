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
//! warning yasb4arm is a "Shared Secret" system, so these keys must be kept SECRET
#define KEY11     0x47F6B74B  
#define KEY12     0x35664A3D
#define KEY13     0xB49CBA17
#define KEY14     0xBC430E26

//! brief Keys2 used in random IV cbc mode encryption.
//! warning yasb4arm is a "Shared Secret" system, so these keys must be kept SECRET
#define KEY21     0xE5414805  
#define KEY22     0xA6A87E4F
#define KEY23     0xBBD194BF
#define KEY24     0xB1A28F83

#endif
