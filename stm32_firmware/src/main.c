#include "stm32f1xx.h"
#include "lcd.h"
#include "uart_stream.h"
#include <stdio.h>
#include "led_status.h"


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




volatile struct VehicleDataPacket latest_telemetry_data; //in case of interrupt

static uint8_t curr_enginestatus1 = 1;
static uint8_t curr_enginestatus2 = 1;

void Run_LocationDisplayTask(struct VehicleDataPacket *tracking_data) {

    char text_buffer[20];

    LCD_Clear();

    //Display Latitude
    long lat_deg = tracking_data->latitude / 1000000;
    long lat_frac = tracking_data->latitude % 1000000;
    if (lat_frac < 0) lat_frac = -lat_frac;

    snprintf(text_buffer, sizeof(text_buffer), "Lat:%ld.%06ld", lat_deg, lat_frac);
    LCD_PrintString(text_buffer);

    LCD_SendCommand(0xC0);

    //Display Longitude
    long lon_deg = tracking_data->longitude / 1000000;
    long lon_frac = tracking_data->longitude % 1000000;
    if (lon_frac < 0) lon_frac = -lon_frac;
    
    snprintf(text_buffer, sizeof(text_buffer), "Lon:%ld.%06ld", lon_deg, lon_frac);
    LCD_PrintString(text_buffer);

    LED_Toggle();
}


void CheckEngineStatus(struct VehicleDataPacket *tracking_data){

    uint8_t inc_enginestatus1 = (tracking_data->status) & (1<<1);
    uint8_t inc_enginestatus2 = (tracking_data->status) & (1<<2);
    if (inc_enginestatus1 == 0 || inc_enginestatus2 == 0){
        //trigger interrupt
        curr_enginestatus1 = inc_enginestatus1;
        curr_enginestatus2 = inc_enginestatus2;
    }
    

}

int main (void) {

    struct VehicleDataPacket live_packet;

    UART1_Init();
    LCD_Init();
    LED_Status_Init();
    LCD_Clear();
    LCD_PrintString("Starting...");


    while(1){
        if (UART1_CheckForPacket(&live_packet) == 1){

            latest_telemetry_data = live_packet;

            CheckEngineStatus(&live_packet);
            Run_LocationDisplayTask(&live_packet);
        }
    }

}