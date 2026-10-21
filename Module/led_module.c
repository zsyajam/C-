#include "led_module.h"
#include "bsp_led.h"

static int mode = LED_MODE_MANUAL;
static uint32_t period = 500;
static uint32_t last = 0;

void LED_Module_Init(void)
{
    mode = LED_MODE_MANUAL;
    period = 500;
    BSP_LED_Init();
    last = HAL_GetTick();
}

void LED_Module_SetMode(int m)
{
    if ((m == LED_MODE_MANUAL) || (m == LED_MODE_BLINK)) {
        mode = m;
        last = HAL_GetTick();
    }
}

int LED_Module_GetMode(void)
{
    return mode;
}

void LED_Module_Set(uint8_t on)
{
    if (on) BSP_LED_On();
    else BSP_LED_Off();
}

void LED_Module_Toggle(void)
{
    BSP_LED_Toggle();
}

uint8_t LED_Module_GetState(void)
{
    return BSP_LED_GetState();
}

void LED_Module_SetPeriod(uint32_t period_ms)
{
    if ((period_ms >= 100) && (period_ms <= 5000)) period = period_ms;
}

uint32_t LED_Module_GetPeriod(void)
{
    return period;
}

void LED_Module_Tick(void)
{
    uint32_t now = HAL_GetTick();
    if ((mode == LED_MODE_BLINK) && ((uint32_t)(now - last) >= period)) {
        last = now;
        BSP_LED_Toggle();
    }
}
