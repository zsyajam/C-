#include "bsp_led.h"

static uint8_t led_state = 0;

void BSP_LED_Init(void)
{
    BSP_LED_Off();
}

void BSP_LED_On(void)
{
    /* Blue Pill PC13 board LED is active low. */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    led_state = 1;
}

void BSP_LED_Off(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    led_state = 0;
}

void BSP_LED_Toggle(void)
{
    if (led_state) BSP_LED_Off();
    else BSP_LED_On();
}

uint8_t BSP_LED_GetState(void)
{
    return led_state;
}
