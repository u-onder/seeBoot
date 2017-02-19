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

#define k1 "47F6B74B35664A3DB49CBA17BC430E26"
#define k2 "E5414805A6A87E4FBBD194BFB1A28F83"

#endif
