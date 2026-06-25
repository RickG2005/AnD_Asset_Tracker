#ifndef LCD_H
#define LCD_H

#include "stm32f1xx.h"

// Backlight and Control Pin Bitmasks matching your working code
#define LCD_BACKLIGHT 0x08
#define LCD_PIN_RS    0x01
#define LCD_PIN_EN    0x04

// Core Driver Functions
void LCD_WriteNibble(uint8_t data, uint8_t control_flags);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendChar(char ch);
void LCD_PrintString(char *str);
void LCD_Clear(void);

#endif