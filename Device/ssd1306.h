#ifndef SSD1306_H
#define SSD1306_H

#include "main.h"

#define SSD1306_WIDTH 128U
#define SSD1306_HEIGHT 64U

HAL_StatusTypeDef SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_DrawString(uint8_t x, uint8_t y, const char *text);
HAL_StatusTypeDef SSD1306_Update(void);

#endif
