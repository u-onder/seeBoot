#ifndef _CIPHER_CONFIG_H_
#define _CIPHER_CONFIG_H_

//! def CIPHER_ROUNDS
//! brief The round of Feistel network. 
//! brief XTEA's offical definition says it should be 32
//!       But, it can be less if dechiper process takes too long
//! warning It should be greater or equal than 3 for security reasons
#define CIPHER_ROUNDS   (32)

#endif
