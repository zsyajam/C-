#include "command.h"
#include "uart_line.h"
#include "led_module.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static uint32_t ok_count;
static uint32_t error_count;

void Command_Init(void)
{
    ok_count = 0;
    error_count = 0;
}

uint32_t Command_GetOkCount(void) { return ok_count; }
uint32_t Command_GetErrorCount(void) { return error_count; }

static void Normalize(char *s)
{
    char *read = s;
    char *write = s;
    uint8_t space = 0;
    while (isspace((unsigned char)*read)) read++;
    while (*read) {
        if (isspace((unsigned char)*read)) {
            space = 1;
        } else {
            if (space && (write != s)) *write++ = ' ';
            *write++ = toupper((unsigned char)*read);
            space = 0;
        }
        read++;
    }
    *write = '\0';
}

static void SendStatus(void)
{
    char buf[128];
    sprintf(buf, "MODE=%s LED=%s PERIOD=%lums COUNT=%lus OK=%lu ERR=%lu\r\n",
            LED_Module_GetMode() == LED_MODE_MANUAL ? "MANUAL" : "BLINK",
            LED_Module_GetState() ? "ON" : "OFF",
            (unsigned long)LED_Module_GetPeriod(),
            (unsigned long)(HAL_GetTick() / 1000),
            (unsigned long)ok_count,
            (unsigned long)error_count);
    UART_Line_Send(buf);
}

static void Execute(char *line)
{
    Normalize(line);

    if (strcmp(line, "HELP") == 0)
    {
        ok_count++;
        UART_Line_Send(
            "HELP | LED ON | LED OFF | LED TOGGLE | STATUS\r\n");
    }
    else if (strcmp(line, "LED ON") == 0)
    {
        LED_Module_SetMode(LED_MODE_MANUAL);
        LED_Module_Set(1);
        ok_count++;
        UART_Line_Send("OK LED=ON\r\n");
    }
    else if (strcmp(line, "LED OFF") == 0)
    {
        LED_Module_SetMode(LED_MODE_MANUAL);
        LED_Module_Set(0);
        ok_count++;
        UART_Line_Send("OK LED=OFF\r\n");
    }
    else if (strcmp(line, "LED TOGGLE") == 0)
    {
        LED_Module_SetMode(LED_MODE_MANUAL);
        LED_Module_Toggle();
        ok_count++;

        if (LED_Module_GetState())
        {
            UART_Line_Send("OK LED=ON\r\n");
        }
        else
        {
            UART_Line_Send("OK LED=OFF\r\n");
        }
    }
    else if (strcmp(line, "STATUS") == 0)
    {
        ok_count++;
        SendStatus();
    }
    else if (line[0] == '\0')
    {
        /* ????,CRLF ?????? */
    }
    else
    {
        error_count++;
        UART_Line_Send("ERROR: UNKNOWN COMMAND\r\n");
    }
}

void Command_Task(void)
{
    char line[UART_LINE_MAX + 1];
    if (UART_Line_GetOverflow()) {
        error_count++;
        UART_Line_Send("ERROR: COMMAND TOO LONG\r\n");
    }
    if (UART_Line_Get(line, sizeof(line))) Execute(line);
}
