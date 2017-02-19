#ifndef _CIPHER_H_
#define _CIPHER_H_

typedef unsigned int ui32_t;
typedef unsigned int uint_t;
typedef   signed int  int_t;

void blockCipher	(ui32_t* iv, ui32_t* key1, ui32_t* key2, ui32_t* v, int_t len);
void blockDecipher	(ui32_t* iv, ui32_t* key1, ui32_t* key2, ui32_t* v, int_t len);

#endif
