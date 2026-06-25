#include "lcd.h"

// Low-level function to stream a nibble and toggle the Enable pin (0x04)
void LCD_WriteNibble(uint8_t data, uint8_t control_flags) {
    // 1. Send data with Enable HIGH
    I2C1->DR = (data | LCD_BACKLIGHT | control_flags | LCD_PIN_EN);
    while (!(I2C1->SR1 & (1 << 7)));
    for(volatile int d = 0; d < 1000; d++);

    // 2. Send data with Enable LOW (falling edge latches the command)
    I2C1->DR = (data | LCD_BACKLIGHT | control_flags);
    while (!(I2C1->SR1 & (1 << 7)));
    for(volatile int d = 0; d < 1000; d++);
}

// Sends system control commands (RS pin = 0x00)
void LCD_SendCommand(uint8_t cmd) {
    uint8_t higher_nibble = cmd & 0xF0;
    uint8_t lower_nibble = (cmd << 4) & 0xF0;

    LCD_WriteNibble(higher_nibble, 0x00);
    LCD_WriteNibble(lower_nibble, 0x00);
}

// Sends a single data text character (RS pin = 0x01)
void LCD_SendChar(char ch) {
    uint8_t higher_nibble = ch & 0xF0;
    uint8_t lower_nibble = (ch << 4) & 0xF0;

    LCD_WriteNibble(higher_nibble, LCD_PIN_RS);
    LCD_WriteNibble(lower_nibble, LCD_PIN_RS);
}

// Loops through an array until it hits the null-terminator
void LCD_PrintString(char *str) {
    while (*str) {
        LCD_SendChar(*str++);
    }
}

// Dedicated command loop to wipe the glass text workspace clean
void LCD_Clear(void) {
    LCD_SendCommand(0x01);
    for(volatile int d = 0; d < 50000; d++); // Clear requires a long cushion delay
}