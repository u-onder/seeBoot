#ifndef _CONFIG_H_
#define _CONFIG_H_

#define FlashSize_d				(1024*1024)

#define FlashAddressOfset_d		(0x08000000)

#define FlashBootAddress_d		(0x08004000)
#define FlashSignatureAddress_d	(0x080FFFE0)
#define FlashHashAddress_d		(0x080FFFF0)
#define FlashEndAddress_d		(0x080FFFFF)

#define MaximumBlockSize_d		(64)

#define CIPHER_ROUNDS	32

#define k1 "00112233445566778899AABBCCDDEEFF"
#define k2 "0123456789ABCDEFFEDCBA9876543210"

#endif
