#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "crc.h"

#include "cipher.h"

// sample image name
// "<?xml version=\"1.0\"  standalone=\'no\'?>\r\n"
// "<FIRMWARE>\r\n"
// "<VERSION NAME=\"TEST FIRM V1\"/>\r\n"
// "<block>84B80B9CC14888EF6C301ACD4CB44E73DB4899A43B806DC84949EED860C07036A99996E2F034B93C71D11A41E9711EB38115802E7D39971A0EC8B9EED1B15FCE033563031C02B31C6A69C9BDF162CA0A</block>"
// "</FIRMWARE>"

// block is configured as follows
// <randomIV[8]><address[4]><payload[64]><CRC[4]> = 80 chars = 160 ascii coded decimal
int generateImage(char* buf, char* xmlfilename, char* imagenamestr, unsigned int* key1, unsigned int* key2, int enc)
{
	FILE* xf;
	//---------DEBUG---------->
	//FILE* tf;

	unsigned int add;
	unsigned int random1, random2;
	unsigned int crc;
	unsigned int i,j;

	typedef union {
	int  alignDummy[1]; //32 bit aligner
	unsigned char str[300];	//we will use 280 bytes
	} bstr_u;

	bstr_u bstr;

	srand((unsigned int) time(NULL));	//initialize random number generator

	xf = fopen(xmlfilename, "wt");
	if (xf == (FILE*)0)
	{
		printf("ERROR: Could not open target image file \"%s\"", xmlfilename);
		return -1;
	}

	fprintf(xf, "<?xml version=\"1.0\"  standalone=\'no\'?>\r\n");
	fprintf(xf, "<FIRMWARE NAME=\"%s\">\r\n", imagenamestr);

	//---------DEBUG---------->
	//tf = fopen("temp.txt", "wt");

	add = FlashAddressOfset_d;

	while (add < FlashEndAddress_d)
	{
		while (add < FlashEndAddress_d)
		{
			if (buf[add - FlashAddressOfset_d] != (char)0xFF)
				break;
			add++;
		}

		if (add >= FlashEndAddress_d)
			break;

		fprintf(xf, "<block>");

		random1  = (rand() & 0xFFF);
		random1 <<= 12;
		random1 += (rand() & 0xFFF);
		random1 <<= 8;
		random1 += (rand() & 0x0FF);
		random2  = (rand() & 0xFFF);
		random2 <<= 12;
		random2 += (rand() & 0xFFF);
		random2 <<= 8;
		random2 += (rand() & 0x0FF);

		add = add & 0xFFFFFFFF - MaximumBlockSize_d + 1;	//align address to BlockSize bytes
		
		bstr.str[0]  = (random1 >>  0) & 0xFF;
		bstr.str[1]  = (random1 >>  8) & 0xFF;
		bstr.str[2]  = (random1 >> 16) & 0xFF;
		bstr.str[3]  = (random1 >> 24) & 0xFF;
		bstr.str[4]  = (random2 >>  0) & 0xFF;
		bstr.str[5]  = (random2 >>  8) & 0xFF;
		bstr.str[6]  = (random2 >> 16) & 0xFF;
		bstr.str[7]  = (random2 >> 24) & 0xFF;
		bstr.str[8]  = (add     >>  0) & 0xFF;
		bstr.str[9]  = (add     >>  8) & 0xFF;
		bstr.str[10] = (add     >> 16) & 0xFF;
		bstr.str[11] = (add     >> 24) & 0xFF;

		for (i=0; i< MaximumBlockSize_d / 4; i++)
		{
			for (j=0; j<4; j++)
			{
				bstr.str[12 + i * 4 + j] = buf[add - FlashAddressOfset_d];
				add++;
			}
		}

		crc = CRC_CalcBlockCRC((unsigned int*)(bstr.str), (MaximumBlockSize_d + 12)/4); 
		bstr.str[MaximumBlockSize_d + 12 + 0] = (crc >>  0) & 0xFF;
		bstr.str[MaximumBlockSize_d + 12 + 1] = (crc >>  8) & 0xFF;
		bstr.str[MaximumBlockSize_d + 12 + 2] = (crc >> 16) & 0xFF;
		bstr.str[MaximumBlockSize_d + 12 + 3] = (crc >> 24) & 0xFF;
		
		//---------DEBUG---------->
        //for (i = 0; i < (MaximumBlockSize_d + 12 + 4); i++)
        //	fprintf(tf, "%02X ", bstr.str[i]);
        //fprintf(tf,"\r\n");

		if (enc == 1)
			blockCipher(((unsigned int*)bstr.str), key1, key2, ((unsigned int*)(bstr.str + 8)), (MaximumBlockSize_d + 12 + 4 - 8)/4);
		//Test chipher
		//blockDecipher(((unsigned int*)bstr.str), key1, key2, ((unsigned int*)(bstr.str + 8)), (MaximumBlockSize_d + 12 + 4 - 8)/4);

		for (i = 0; i < (MaximumBlockSize_d + 12 + 4); i++)
			fprintf(xf, "%02X", bstr.str[i]);
		fprintf(xf, "</block>\r\n");

		//---------DEBUG---------->
        //blockDecipher(((unsigned int*)bstr.str), key1, key2, ((unsigned int*)(bstr.str + 8)), (MaximumBlockSize_d + 12 + 4 - 8)/4);
        //for (i = 0; i < (MaximumBlockSize_d + 12 + 4); i++)
        //	fprintf(tf, "%02X ", bstr.str[i]);
        //fprintf(tf,"\r\n");
	}

	fprintf(xf, "</FIRMWARE>");
	fclose (xf);
	return 0;
}
