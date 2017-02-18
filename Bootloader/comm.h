#ifndef _COMM_H_
#define _COMM_H_

#include "config.h"

#include "cpu.h"

int_t   commInit        (void);
int_t   commDispose     (void);
int_t   commSendHeader  (void);
int_t   commSendBody    (char_t* str);
int_t   commSendFooter  (int_t sendCRC);
int_t   commFlush       (void);
char_t* commParse       (void);

#endif
