#include "led_module.h"
#include "bsp_led.h"

int mode = 0;

void LED_Module_Init(void)
{
    mode = 0;
    BSP_LED_Off();
}

void LED_Module_SetMode(int m)
{
    mode = m;
}

int LED_Module_GetMode(void)
{
    return mode;
}

void LED_Module_Tick(void)
{
    static uint32_t last = 0;

    if (mode == 1)
    {
        if (HAL_GetTick() - last >= 500)
        {
            last = HAL_GetTick();
            BSP_LED_Toggle();
        }
    }
}