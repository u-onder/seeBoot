#ifndef _CYPHER_H_
#define _CYPHER_H_

#include "cpu.h"

void   blockCipher    (ui32_t* iv, ui32_t* key1, ui32_t* key2, ui32_t* v, int_t len);
void   blockDecipher  (ui32_t* iv, ui32_t* key1, ui32_t* key2, ui32_t* v, int_t len);

#endif
