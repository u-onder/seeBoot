#include <stdio.h>
#include <string.h>

#include "config.h"

#include "cipher.h"
#include "misc.h"
#include "parser.h"
#include "crc.h"
#include "igenerator.h"

union flashUnion{
	unsigned int alignerDummy[1];
	char flash[FlashSize_d/16][16];	//1 MB
} flash;


int main (int argc, char** argv)
{
	char         hexfilename[1024];
	char         outfilename[1024];
	unsigned int key1[4];
	unsigned int key2[4];
	char         imagenamestring[1024];

	unsigned int crc;

	int arg;
	int flag;

	printImgGenHeader();

	if (argc < 7)
	{
		printImgGenUsage();
		return -1;
	}

	arg = 1;
	flag = 0;
	while (arg < argc)
	{
		if (strcmp(argv[arg], "-f") == 0)
		{
			getSourceFileName(hexfilename, argv[arg+1]);
			arg += 2;
			flag |= 0x0001;
			continue;
		}
		if (strcmp(argv[arg], "-o") == 0)
		{
			getTargetFileName(outfilename, argv[arg+1]);
			arg += 2;
			flag |= 0x0002;
			continue;
		}
		if (strcmp(argv[arg], "-i") == 0)
		{
			getImageName(imagenamestring, argv[arg+1]);
			arg += 2;
			flag |= 0x0004;
			continue;
		}
		printf("ERROR: invalid argument \"%s\"", argv[arg]);
		return -1;
	}

	getKey(key1, k1);
	getKey(key2, k2);

	if ((flag & 0x0007) != 0x0007)
	{
		printf("ERROR: Missing parameters\r\n");
		return -1;
	}

	printf("  Hex file: %s\r\n", hexfilename);
	printf("  Out file: %s\r\n", outfilename);
	printf("Image name: %s\r\n", imagenamestring);
	printf("\r\nProcessing...\r\n");

	//parse input file
	parseInputFile((char*)flash.flash, hexfilename);

	//mark binary
	sprintf(((char*)flash.flash)+(FlashSignatureAddress_d - FlashAddressOfset_d), "%s", imagenamestring);
	crc = CRC_CalcBlockCRC((unsigned int*)(((char*)flash.flash)+(FlashBootAddress_d - FlashAddressOfset_d)), 
		             (FlashHashAddress_d - FlashBootAddress_d)/4); 
	((char*)flash.flash)[FlashHashAddress_d - FlashAddressOfset_d+0] = (crc & 0x000000FF)>>0;
	((char*)flash.flash)[FlashHashAddress_d - FlashAddressOfset_d+1] = (crc & 0x0000FF00)>>8;
	((char*)flash.flash)[FlashHashAddress_d - FlashAddressOfset_d+2] = (crc & 0x00FF0000)>>16;
	((char*)flash.flash)[FlashHashAddress_d - FlashAddressOfset_d+3] = (crc & 0xFF000000)>>24;

	//convert to xml
	generateImage((char*)flash.flash, outfilename, imagenamestring, key1, key2, 1);

	printf("\r\nImage successfully generated\r\n\r\n");
}
