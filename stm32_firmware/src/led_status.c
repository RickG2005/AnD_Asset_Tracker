#include "led_status.h"
#include <stddef.h>

void LED_Status_Init(void) {
    // 1. HARDWARE TIMER SETUP (TIMER 2)
    RCC->APB1ENR |= (1 << 0);    // Turn on clock gate for Timer 2
    TIM2->PSC = 8000 - 1;        // Scale clock down to 1ms per tick
    TIM2->ARR = 500;             // Set target count to 500ms
    TIM2->EGR |= (1 << 0);       // Force update to load prescaler
    TIM2->CR1 |= (1 << 0);       // Start Timer 2

    // 2. GPIO SETUP (PORT C PIN 13)
    RCC->APB2ENR |= (1 << 4);    // Turn on clock gate for Port C
    GPIOC->CRH &= ~(15 << 20);   // Clear configuration for Pin 13
    GPIOC->CRH |= (2 << 20);     // Set Pin 13 to Output Push-Pull
}

void LED_Toggle(void) {   
        GPIOC->ODR ^= (1 << 13);   // Toggle the status LED
}