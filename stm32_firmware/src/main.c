#include "stm32f1xx.h"
#include "lcd.h"


extern void _estack(void);
__attribute__((naked)) void Reset_Handler(void) {
    __asm volatile(
        "ldr r0, =_estack \n"
        "mov sp, r0       \n"
        "b main           \n"
    );
}

int main (void) {
  
  RCC->APB2ENR |= (1<<3);
  RCC->APB1ENR |= (1<<21);

  GPIOB->CRL &= ~(15 << 24);
  GPIOB->CRL |= (14<<24);
  
  GPIOB->CRL &= ~(15 << 28);
  GPIOB->CRL |= (14<<28);
  
  I2C1->CR2 = 36;
  I2C1->CCR = 180;
  I2C1->TRISE = 37;

  I2C1->CR1 |= (1<<0);
  I2C1->CR1 |= (1<<8);

  while (!(I2C1->SR1 & 1)){
    
  }

  I2C1->DR = (0x27<<1);
  while (!(I2C1->SR1 & 2)){
    
  }

  volatile uint32_t temp;
  temp = I2C1->SR1;
  temp = I2C1->SR2;

  for(volatile int i = 0; i < 300000; i++); 

  LCD_WriteNibble(0x20, 0x00); // Force 4-Bit Mode
  LCD_SendCommand(0x28);       // Function Set: 2 Lines, 5x8 Font
  LCD_SendCommand(0x0C);       // Display ON, Cursor OFF
  LCD_Clear();                 // Clear Screen

  LCD_PrintString("Rick is goated?");

  I2C1->CR1 |= (1 << 9);

}
