#ifndef __UART_LINE_H
#define __UART_LINE_H

#include "main.h"

#define UART_LINE_MAX 64

void UART_Line_Init(void);
void UART_Line_RxCallback(UART_HandleTypeDef *huart);
int UART_Line_Get(char *out, uint16_t out_size);
int UART_Line_GetOverflow(void);
void UART_Line_Send(const char *text);

#endif
