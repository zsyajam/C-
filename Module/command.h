#ifndef __COMMAND_H
#define __COMMAND_H

#include "main.h"

void Command_Init(void);
void Command_Task(void);
uint32_t Command_GetOkCount(void);
uint32_t Command_GetErrorCount(void);

#endif
