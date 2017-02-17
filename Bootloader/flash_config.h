#ifndef _FLASHCONFIG_H_
#define _FLASHCONFIG_H_

//! \def BOOTIMAGE_NAME_SIZE
//! \brief Maximum size of boot image definition string
#define BOOTIMAGE_NAME_SIZE  (16)

//! \def BOOTIMAGE_START_ADDRESS
//! \brief Start address of the loaded program.
//! \brief This is also the bootloader jump address
#define BOOTIMAGE_START_ADDRESS         0x08004000

//! \def BOOTIMAGE_SIGNATURE_ADDRESS
//! \brief The signature or Image name string address
#define BOOTIMAGE_SIGNATURE_ADDRESS     0x080FFFE0

//! \def BOOTIMAGE_HASH_ADDRESS
//! \brief Address of the hash (here crc32) of all code space
#define BOOTIMAGE_HASH_ADDRESS          0x080FFFF0

#endif
