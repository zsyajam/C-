#include "app_task.h"
#include "key.h"
#include "led_module.h"
#include "uart_line.h"
#include "command.h"
#include "ssd1306.h"
#include <stdio.h>

static uint32_t key_tick;
static uint32_t oled_tick;
static uint8_t oled_ok;

/* KEY2 选择的待确认 LED 状态 */
static uint8_t pending_led;
static uint8_t pending_valid;

static void HandleKey(int key)
{
    if (key == 1)
    {
        /* KEY1：切换 MANUAL / BLINK 模式 */
        if (LED_Module_GetMode() == LED_MODE_MANUAL)
        {
            LED_Module_SetMode(LED_MODE_BLINK);
        }
        else
        {
            LED_Module_SetMode(LED_MODE_MANUAL);
        }

        /* 切换模式时取消未确认的操作 */
        pending_valid = 0;
    }
    else if (key == 2)
    {
        /*
         * KEY2：选择 LED ON/OFF，但暂时不执行。
         * 第一次按下选择与当前状态相反的状态；
         * 再按一次继续切换待确认状态。
         */
        LED_Module_SetMode(LED_MODE_MANUAL);

        if (pending_valid)
        {
            pending_led = !pending_led;
        }
        else
        {
            pending_led = !LED_Module_GetState();
            pending_valid = 1;
        }
    }
    else if (key == 3)
    {
        /* KEY3：确认 KEY2 选择的状态 */
        if (pending_valid)
        {
            LED_Module_SetMode(LED_MODE_MANUAL);
            LED_Module_Set(pending_led);
            pending_valid = 0;
        }
    }
}

static void OLED_ShowMainPage(void)
{
    char line[22];

    SSD1306_Clear();

    SSD1306_DrawString(0, 0, "STM32 CONTROL");

    sprintf(line, "MODE: %s",
        LED_Module_GetMode() == LED_MODE_MANUAL
        ? "MANUAL"
        : "BLINK");
    SSD1306_DrawString(0, 16, line);

    if (pending_valid)
    {
        /* 显示实际状态和待确认状态 */
        sprintf(line, "LED: %s > %s?",
            LED_Module_GetState() ? "ON" : "OFF",
            pending_led ? "ON" : "OFF");
    }
    else
    {
        sprintf(line, "LED: %s",
            LED_Module_GetState() ? "ON" : "OFF");
    }

    SSD1306_DrawString(0, 32, line);

    sprintf(line, "COUNT: %04lu",
        (unsigned long)(HAL_GetTick() / 1000));
    SSD1306_DrawString(0, 48, line);

    (void)SSD1306_Update();
}

void Task_Init(void)
{
    LED_Module_Init();
    Key_Init();
    Command_Init();
    UART_Line_Init();

    pending_led = 0;
    pending_valid = 0;

    oled_ok = (SSD1306_Init() == HAL_OK) ? 1 : 0;

    key_tick = HAL_GetTick();
    oled_tick = key_tick;

    UART_Line_Send("STM32 CONTROL READY V1.0\r\n");

    if (oled_ok)
    {
        OLED_ShowMainPage();
    }
}

void Task_Run(void)
{
    uint32_t now = HAL_GetTick();
    int key;

    while ((uint32_t)(now - key_tick) >= 10)
    {
        key_tick += 10;
        Key_Scan10ms();
    }

    key = Key_GetNum();

    if (key != 0)
    {
        HandleKey(key);
    }

    LED_Module_Tick();
    Command_Task();

    if (oled_ok &&
        ((uint32_t)(now - oled_tick) >= 200))
    {
        oled_tick = now;
        OLED_ShowMainPage();
    }
}

void Task_UART_RxCallback(UART_HandleTypeDef* huart)
{
    UART_Line_RxCallback(huart);
}