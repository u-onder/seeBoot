#ifndef _TIMER_H_
#define _TIMER_H_

#include "cpu.h"

int_t  timerInit  (void);
int_t  timerClose (void);

void   timerSet   (ui32_t timeout);
ui32_t timerGet   (void);

#endif
