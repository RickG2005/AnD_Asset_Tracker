#include "stm32f1xx.h"
#include "lcd.h"
#include "uart_stream.h"


extern void _estack(void);
__attribute__((naked)) void Reset_Handler(void) {
    __asm volatile(
        "ldr r0, =_estack \n"
        "mov sp, r0       \n"
        "b main           \n"
    );
}

int main (void) {
    UART1_Init();
    LCD_Init();

    struct VehicleDataPacket tracking_data;
    char text_buffer[20];

    while(1){
        while (UART1_CheckForPacket(&tracking_data) == 0){
        //do nothing
        }

        sprintf(text_buffer, "Lon: %ld", tracking_data.longitude);
        LCD_Clear();
        LCD_PrintString(text_buffer);
    }
}