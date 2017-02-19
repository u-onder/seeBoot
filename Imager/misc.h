#ifndef _MISC_H_
#define _MISC_H_

int			 isBase16Char(char X);
unsigned int hex2dec(char X);

int getSourceFileName(char* hexfilename, char* argv);
int getTargetFileName(char* xmlfilename, char* argv);
int getKey(unsigned int* key, char* argv);
int getImageName(char* imagenamestring, char* argv);

void printImgGenHeader(void);
void printImgGenUsage(void);

#endif
