#include <stdio.h>
#include <string.h>

int isBase16Char(char X)
{
	if ((X >= '0') && (X <= '9'))
		return 0;
	if ((X >= 'a') && (X <= 'f'))
		return 0;
	if ((X >= 'A') && (X <= 'F'))
		return 0;
	return -1;
}
unsigned int hex2dec(char X)
{
	if ((X >= '0') && (X <= '9'))
		return X-'0';
	if ((X >= 'a') && (X <= 'f'))
		return X-'a'+10;
	if ((X >= 'A') && (X <= 'F'))
		return X-'A'+10;
	return 0;
}

int getSourceFileName(char* hexfilename, char* argv)
{
	FILE* file;
	file = fopen(argv,"rb");
	if (file == (FILE*)0)
	{
		printf("ERROR: Couldn't find hex file \'%s\'\r\n", argv);
		hexfilename[0] = '\0';
		return -1;
	}
	fclose(file);
	sprintf(hexfilename, "%s", argv);
	return 0;
}

int getTargetFileName(char* xmlfilename, char* argv)
{
	sprintf(xmlfilename, "%s", argv);
	return 0;
}


int getKey(unsigned int* key, char* argv)
{
	int i;
	key[0] = key[1] = key[2] = key[3] = 0;
	if (strlen(argv) != 32)
	{
		printf("ERROR: Invalid key or keylength!\r\n");
		return -1;
	}
	for (i = 0; i < 32; i++)
	{
		if (isBase16Char(argv[i]) == -1)
		{
			printf("ERROR: Invalid key or keylength!\r\n");
			return -1;
		}
		key[i/8] += hex2dec(argv[i]) << (28 - (i % 8)*4);
	}
	return 0;
}
int getImageName(char* imagenamestring, char* argv)
{
	if (strlen(argv) > 16)
	{
		printf("ERROR: Image name string is too long!\r\n");
		imagenamestring[0] = '\0';
		return -1;
	}
	sprintf(imagenamestring, "%s", argv);
	return 0;
}

void printImgGenHeader(void)
{
	printf("\r\n");
	printf("-----------------------------------------------------------------\r\n");
	printf("-----------------------------------------------------------------\r\n");
	printf("yasb4arm Image Generation Utility for ARM - IGenerator v0.1\r\n");
	printf("-----------------------------------------------------------------\r\n");
	printf("This tool is to generate encrypted releases (image files) for\r\n");
	printf("ARM devices, supporting \"yasb4arm bootloader\"\r\n");
	printf("\r\n");
	printf("You can use \"IGenerator.exe\" command for usage information.");
	printf("\r\n");
	printf("-----------------------------------------------------------------\r\n");
	printf("-----------------------------------------------------------------\r\n");
}

void printImgGenUsage(void)
{
	printf("Usage:\r\n");
	printf("IGenerator.exe -f <hexfile> -o <outfile> -i <iname>\r\n");
	printf("\r\n");
	printf("Where:\r\n");
	printf(" <hexfile> is intelextended formatted firmware binary\r\n");
	printf(" <outfile> is digitally signed output file\r\n");
	printf(" <iname>   is release name, max 16 characters long\r\n");
}
