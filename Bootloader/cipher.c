/*! \file cipher.c
    \brief   seeBoot cipher algorithm
	
	\details seeBoot cipher algorithm
	\details seeBoot uses XTEA algorithm as cipher. This algorithm takes two 32 bit words and does
             CHIPHER_ROUDS Feistel operations. Key space is four 32 bit words.

	\details seeBoot uses CBC with random IV mode. To deal with choosen cipher text attacks,
			 we use a second four 32 bit word key to encrypt IV before plain CBC.

	\date      2017
	\author    u-onder
	\copyright MIT License
*/
#include "config.h"

#include "cipher.h"

//local functions
void __cipher   (ui32_t v[2], ui32_t key[4]);
void __decipher (ui32_t v[2], ui32_t key[4]);

//cipher routine
void blockCipher(ui32_t* iv, ui32_t* key1, ui32_t* key2, ui32_t* v, int_t len)
{
	ui32_t eiv[2]; //enc iv
	ui32_t civ[2]; //current iv

	int_t i;

	eiv[0] = iv[0]; eiv[1] = iv[1];
	
	__cipher(eiv,key2);
	civ[0] = eiv[0]; civ[1] = eiv[1];

	for (i = 0; i < len; i += 2)
	{
		v[i]   ^= civ[0];
		v[i+1] ^= civ[1];

		__cipher(v+i, key1);

		civ[0] = v[i];
		civ[1] = v[i+1];
	}
}

//decipher routine
void blockDecipher(ui32_t* iv, ui32_t* key1, ui32_t* key2, ui32_t* v, int_t len)
{
	ui32_t eiv[2]; //enc iv
	ui32_t civ[2]; //current iv

	int_t i;

	eiv[0] = iv[0]; eiv[1] = iv[1];
	
	__cipher(eiv,key2);

	for (i = 0; i < len - 2; i += 2)
	{
		__decipher(v+len-2-i, key1);
		civ[0] = v[len-4-i];
		civ[1] = v[len-4-i+1];
		v[len-2-i]   ^= civ[0];
		v[len-2-i+1] ^= civ[1];
	}

	__decipher(v, key1);
	civ[0] = eiv[0]; civ[1] = eiv[1];
	v[0] ^= civ[0];
	v[1] ^= civ[1];
}

//local call only single block cipher
void __cipher(ui32_t v[2], ui32_t key[4]) 
{
    uint_t i;
    ui32_t v0=v[0], 
           v1=v[1], 
           sum=0, 
           delta=0x9E3779B9;
    
    for (i=0; i < CIPHER_ROUNDS; i++) 
    {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; 
    v[1]=v1;
}
 
 //local call only single block decipher
void __decipher(ui32_t v[2], ui32_t key[4]) 
{
    uint_t i;
    ui32_t v0=v[0], 
           v1=v[1], 
           delta=0x9E3779B9, 
           sum=delta*CIPHER_ROUNDS;
    
    for (i=0; i < CIPHER_ROUNDS; i++) 
    {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; 
    v[1]=v1;
}