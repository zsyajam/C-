#include "uart_line.h"
#include <string.h>

extern UART_HandleTypeDef huart1;

#define LINE_QUEUE_SIZE 4

static uint8_t rx_byte;
static volatile char building[UART_LINE_MAX + 1];
static volatile uint16_t building_len;
static volatile uint8_t discarding;
static volatile uint8_t overflow_count;
static volatile char queue[LINE_QUEUE_SIZE][UART_LINE_MAX + 1];
static volatile uint8_t head, tail, count;
static uint8_t ignore_lf;

void UART_Line_Init(void)
{
    building_len = 0;
    discarding = 0;
    overflow_count = 0;
    head = tail = count = 0;
    ignore_lf = 0;
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
}

static void UART_Line_Push(void)
{
    uint16_t i;
    if (count >= LINE_QUEUE_SIZE) {
        if (overflow_count < 255) overflow_count++;
        building_len = 0;
        return;
    }
    for (i = 0; i < building_len; i++) queue[head][i] = building[i];
    queue[head][building_len] = '\0';
    head = (head + 1) % LINE_QUEUE_SIZE;
    count++;
    building_len = 0;
}

/* Call this from HAL_UART_RxCpltCallback. */
void UART_Line_RxCallback(UART_HandleTypeDef *huart)
{
    uint8_t ch;
    if (huart != &huart1) return;
    ch = rx_byte;
    if ((ch == '\r') || (ch == '\n')) {
        if ((ch == '\n') && ignore_lf) {
            ignore_lf = 0;
        } else {
            ignore_lf = (ch == '\r');
            if (discarding) {
                discarding = 0;
                building_len = 0;
                if (overflow_count < 255) overflow_count++;
            } else {
                UART_Line_Push();
            }
        }
    } else {
        ignore_lf = 0;
        if (!discarding) {
            if (building_len < UART_LINE_MAX) building[building_len++] = ch;
            else discarding = 1;
        }
    }
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
}

int UART_Line_Get(char *out, uint16_t out_size)
{
    uint16_t i;
    int result = 0;
    if ((out == 0) || (out_size == 0)) return 0;
    __disable_irq();
    if (count) {
        for (i = 0; (i + 1 < out_size) && queue[tail][i]; i++) out[i] = queue[tail][i];
        out[i] = '\0';
        tail = (tail + 1) % LINE_QUEUE_SIZE;
        count--;
        result = 1;
    }
    __enable_irq();
    return result;
}

int UART_Line_GetOverflow(void)
{
    int result = 0;
    __disable_irq();
    if (overflow_count) {
        overflow_count--;
        result = 1;
    }
    __enable_irq();
    return result;
}

void UART_Line_Send(const char *text)
{
    if (text) HAL_UART_Transmit(&huart1, (uint8_t *)text, strlen(text), 100);
}
