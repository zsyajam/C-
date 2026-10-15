#include "key.h"

void Key_Init(void)
{

}

int Key_GetNum(void)
{
    int key = 0;

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
    {
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
        {
            key = 1;
            while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET);
        }
    }

    return key;
}