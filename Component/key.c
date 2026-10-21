#include "key.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t stable;
    uint8_t sample;
    uint8_t count;
} KeyData;

static KeyData keys[3] = {
    {GPIOA, GPIO_PIN_0, 0, 0, 0},
    {GPIOA, GPIO_PIN_1, 0, 0, 0},
    {GPIOA, GPIO_PIN_2, 0, 0, 0}
};
static volatile int key_num = 0;

static uint8_t Key_IsPressed(KeyData *key)
{
    return HAL_GPIO_ReadPin(key->port, key->pin) == GPIO_PIN_RESET;
}

void Key_Init(void)
{
    uint8_t i;
    key_num = 0;
    for (i = 0; i < 3; i++) {
        keys[i].stable = Key_IsPressed(&keys[i]);
        keys[i].sample = keys[i].stable;
        keys[i].count = 0;
    }
}

/* Call once every 10 ms. Two equal samples give about 20 ms debounce. */
void Key_Scan10ms(void)
{
    uint8_t i;
    for (i = 0; i < 3; i++) {
        uint8_t now = Key_IsPressed(&keys[i]);
        if (now == keys[i].sample) {
            if (keys[i].count < 2) keys[i].count++;
        } else {
            keys[i].sample = now;
            keys[i].count = 1;
        }
        if ((keys[i].count >= 2) && (now != keys[i].stable)) {
            keys[i].stable = now;
            if (now && (key_num == 0)) key_num = i + 1;
        }
    }
}

int Key_GetNum(void)
{
    int result;
    __disable_irq();
    result = key_num;
    key_num = 0;
    __enable_irq();
    return result;
}
