#include "cipher.h"

#include "config.h"

void cipher(ui32_t v[2], ui32_t key[4]) 
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
 
void decipher(ui32_t v[2], ui32_t key[4]) 
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

void blockCipher(ui32_t* iv, ui32_t* key1, ui32_t* key2, ui32_t* v, int_t len)
{
	ui32_t eiv[2]; //enc iv
	ui32_t civ[2]; //current iv

	int_t i;

	eiv[0] = iv[0]; eiv[1] = iv[1];
	
	cipher(eiv,key2);
	civ[0] = eiv[0]; civ[1] = eiv[1];

	for (i = 0; i < len; i += 2)
	{
		v[i]   ^= civ[0];
		v[i+1] ^= civ[1];

		cipher(v+i, key1);

		civ[0] = v[i];
		civ[1] = v[i+1];
	}
}

void blockDecipher(ui32_t* iv, ui32_t* key1, ui32_t* key2, ui32_t* v, int_t len)
{
	ui32_t eiv[2]; //enc iv
	ui32_t civ[2]; //current iv

	int_t i;

	eiv[0] = iv[0]; eiv[1] = iv[1];
	
	cipher(eiv,key2);

	for (i = 0; i < len - 2; i += 2)
	{
		decipher(v+len-2-i, key1);
		civ[0] = v[len-4-i];
		civ[1] = v[len-4-i+1];
		v[len-2-i]   ^= civ[0];
		v[len-2-i+1] ^= civ[1];
	}

	decipher(v, key1);
	civ[0] = eiv[0]; civ[1] = eiv[1];
	v[0] ^= civ[0];
	v[1] ^= civ[1];
}
