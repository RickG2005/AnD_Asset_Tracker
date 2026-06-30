#ifndef UART_STREAM_H
#define UART_STREAM_H

#include "stm32f1xx.h"

struct __attribute__((packed)) VehicleDataPacket {
    int32_t latitude;   // 4 bytes
    int32_t longitude;  // 4 bytes
    uint8_t status;     // 1 byte
    char name[6];       // 6 bytes
};

void UART1_Init(void);
uint8_t UART1_CheckForPacket(struct VehicleDataPacket *out_packet);

#endif 