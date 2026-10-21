#ifndef TASK_APP_TASK_H
#define TASK_APP_TASK_H

#include "main.h"

void Task_Init(void);
void Task_Run(void);
void Task_UART_RxCallback(UART_HandleTypeDef *huart);

#endif
