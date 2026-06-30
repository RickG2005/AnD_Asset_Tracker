#include "stm32f1xx.h"
#include "lcd.h"
#include "uart_stream.h"
#include <stdio.h>


extern void _estack(void);

void Reset_Handler(void);

__attribute__((section(".isr_vector"), used))
void (* const g_pfnVectors[])(void) = {
    &_estack,
    Reset_Handler
};

__attribute__((naked)) void Reset_Handler(void) {
    __asm volatile(
        "ldr r0, =_estack \n"
        "mov sp, r0       \n"
        "b main           \n"
    );
}

int main (void) {
    extern uint8_t rx_index;
    extern uint8_t rx_state;
    rx_index = 0;
    rx_state = 0;

    UART1_Init();
    LCD_Init();
    LCD_Clear();
    LCD_PrintString("Waiting...");

    struct VehicleDataPacket tracking_data;
    char text_buffer[20];

    while(1){
        while (UART1_CheckForPacket(&tracking_data) == 0){
        //do nothing
        }

        LCD_Clear();

        snprintf(text_buffer, sizeof(text_buffer), "Lat:%ld", tracking_data.latitude);
        LCD_PrintString(text_buffer);

        LCD_SendCommand(0xC0);

        snprintf(text_buffer, sizeof(text_buffer), "Lon:%ld", tracking_data.longitude);
        LCD_PrintString(text_buffer);

        
    }
}