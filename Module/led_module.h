#ifndef __LED_MODULE_H
#define __LED_MODULE_H

#include "main.h"

#define LED_MODE_MANUAL 0
#define LED_MODE_BLINK  1

void LED_Module_Init(void);
void LED_Module_SetMode(int m);
int LED_Module_GetMode(void);
void LED_Module_Set(uint8_t on);
void LED_Module_Toggle(void);
uint8_t LED_Module_GetState(void);
void LED_Module_SetPeriod(uint32_t period_ms);
uint32_t LED_Module_GetPeriod(void);
void LED_Module_Tick(void);

#endif
