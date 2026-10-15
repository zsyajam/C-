#ifndef __LED_MODULE_H
#define __LED_MODULE_H

#include "main.h"

void LED_Module_Init(void);
void LED_Module_SetMode(int m);
int LED_Module_GetMode(void);
void LED_Module_Tick(void);

#endif