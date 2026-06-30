#include "lcd.h"
#include "stm32f1xx.h"


void LCD_Init(void) {
    // 1. Enable Clocks for GPIOB and I2C1
    RCC->APB2ENR |= (1 << 3);  // Enable GPIOB Clock (Bit 3)
    RCC->APB1ENR |= (1 << 21); // Enable I2C1 Clock (Bit 21)

    // 2. Configure PB6 (SCL) and PB7 (SDA) as Alternate Function Open-Drain (50MHz)
    // CNF = 11 (Alt Function Open-Drain), MODE = 11 (Output 50MHz) -> 0xF for each pin
    GPIOB->CRL &= ~(0xFF << 24); // Clear configurations for PB6 and PB7
    GPIOB->CRL |= (0xFF << 24);  // Set PB6 and PB7 to 0x4F (Alternate Function Open-Drain)

    // 3. Initialize I2C Peripheral Configurations
    I2C1->CR2 = 36;              // Set I2C peripheral clock frequency (Assuming 36MHz APB1)
    I2C1->CCR = 180;             // Configure clock control for Standard Mode (100kHz)
    I2C1->TRISE = 37;            // Set maximum rise time
    I2C1->CR1 |= (1 << 0);       // Enable I2C1 Peripheral (PE = 1)

    // 4. Generate I2C START condition and send target address
    I2C1->CR1 |= (1 << 8);                     // Generate I2C START condition
    while (!(I2C1->SR1 & (1 << 0)));           // Wait for SB (Start Bit) flag to set
    
    I2C1->DR = (0x27 << 1);                    // Send LCD I2C Address (0x27) shifted for Write
    while (!(I2C1->SR1 & (1 << 1)));           // Wait for ADDR flag to set
    (void)I2C1->SR2;                           // Read SR2 to clear ADDR flag state

    // 5. HD44780 LCD Core Initialization Sequence (4-bit mode)
    for(volatile int d = 0; d < 50000; d++); 

    LCD_WriteNibble(0x30, 0x00);
    for(volatile int d = 0; d < 20000; d++);
    
    LCD_WriteNibble(0x30, 0x00);
    for(volatile int d = 0; d < 5000; d++);
    
    LCD_WriteNibble(0x30, 0x00);
    for(volatile int d = 0; d < 5000; d++);
    
    LCD_WriteNibble(0x20, 0x00);               // Actively switch internal state to 4-bit bus mode
    for(volatile int d = 0; d < 10000; d++);

    // 6. Functional Execution Configurations
    LCD_SendCommand(0x28); // Function Set: 4-bit mode, 2-line display, 5x8 font
    LCD_SendCommand(0x0C); // Display Control: Turn display ON, cursor OFF, blink OFF
    LCD_SendCommand(0x06); // Entry Mode Set: Increment cursor automatically
    
    LCD_Clear();           // Wipe the screen clean
}

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