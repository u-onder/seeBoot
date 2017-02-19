#include <stdio.h>
#include <string.h>

#include "config.h"

#include "misc.h"

#define parserInvalidStartCode_d	(-3)
#define parserInvalidTypeCode_d		(-2)
#define parserError_d				(-1)
#define parserData_d				(0)
#define parserEndOffFile_d			(1)
#define parserOffset_d				(4)

int __parseLine(FILE* f, unsigned int* address, char* buf, unsigned int* count);

unsigned int addressOffset;
char parserBuffer[16];

int parseInputFile(char* buffer, char* hexfilename)
{
	FILE* hf = fopen(hexfilename, "rb");

	unsigned int i;

	int res;

	unsigned int line;
	unsigned int address;
	unsigned int len;

	if (hf == (FILE*)0)
	{
		printf("ERROR: Could not open hex file");
		return -1;
	}

	memset(buffer, 0xFF, FlashSize_d);

	line = 0;
	addressOffset = 0x0000;
	while (1)
	{
		line++;
		// printf(".");
		res = __parseLine(hf, &address, parserBuffer, &len);
	
		//printf("%08X  %08X  %08X\r\n",len, addressOffset + address, res);

		if (res == parserEndOffFile_d)
			return 0;
		if (res == parserError_d)
		{
			printf("WARNING: Unknown error at line %d\r\n", line);
			//do nothing
			//fclose(hf);
			//return -1;
		}
		if (res == parserInvalidStartCode_d)
		{
			printf("ERROR: Invalid start code at line %d", line);
			fclose(hf);
			return -1;
		}
		if (res == parserInvalidTypeCode_d)
		{
			printf("ERROR: Invalid type code at line %d", line);
			fclose(hf);
			return -1;
		}
		if (res == parserOffset_d)
		{
			//do nothing
		}
		if (res == parserData_d)
		{
			char* pb = &buffer[addressOffset - FlashAddressOfset_d + address];
			for (i = 0; i < len; i++)
				pb[i] = parserBuffer[i];
		}
	}
	return 0;
}

int __parseLine(FILE* f, unsigned int* address, char* buf, unsigned int* count)
{
	unsigned char c;

	unsigned int i;
	int type;
	unsigned int cnt;
	int ret = parserError_d;

	do {
		c = fgetc(f);
	} while ((c == '\r') || (c == '\n'));

	if (c != ':')
		return parserInvalidStartCode_d;

	*count   = (hex2dec(fgetc(f)) << 4)  +  hex2dec(fgetc(f));
	*address = (hex2dec(fgetc(f)) << 12) + (hex2dec(fgetc(f)) << 8) + (hex2dec(fgetc(f)) << 4) + hex2dec(fgetc(f));
	type     = (hex2dec(fgetc(f)) << 4)  +  hex2dec(fgetc(f));

	switch (type)
	{
		case 0x00: // data
			cnt = *count;
			for (i = 0; i < cnt; i++)
				buf[i] = (hex2dec(fgetc(f)) << 4) + hex2dec(fgetc(f));
			ret = parserData_d;
			break;
		case 0x01: // end-of-file
			ret = parserEndOffFile_d;
			break;
		case 0x04: // extended linear address
			addressOffset = (hex2dec(fgetc(f)) << 28) + (hex2dec(fgetc(f)) << 24) + (hex2dec(fgetc(f)) << 20) + (hex2dec(fgetc(f)) << 16);
			ret = parserOffset_d;
			break;
		case 0x05: // start linear address
			break;
		case 0x02: // extended segment address
		case 0x03: // start segment address
		default:
			return parserInvalidTypeCode_d;
	}
	do {
		c = fgetc(f);
	}
	while ((c != '\r') && (c != '\n'));
	return ret;
}
