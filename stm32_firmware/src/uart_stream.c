#include "uart_stream.h"
#include <string.h>

static uint8_t rx_buffer[15]; 
uint8_t rx_index = 0;   
int8_t rx_state = 0;

void UART1_Init(void){
    RCC->APB2ENR |= (1<<2);
    RCC->APB2ENR |= (1<<14);

    GPIOA->CRH &= ~(0xFF << 4);
    GPIOA->CRH |= (4<<8);
    GPIOA->CRH |= (11<<4);

    USART1->BRR = 0x341;
    USART1->CR1 |= (1<<13);
    USART1->CR1 |= (1<<2);
}

uint8_t UART1_CheckForPacket(struct VehicleDataPacket *out_packet)
{
    if (!(USART1->SR & (1<<5))){
        return 0;
    }

    uint8_t incoming_byte = USART1->DR;

    switch (rx_state) {
        case 0: 
            if (incoming_byte == '#'){
                rx_index = 0;
                rx_state = 1;
            }
            break;
        case 1:
            rx_buffer[rx_index] = incoming_byte;
            rx_index++; 
            if (rx_index >= 15) {

                memcpy(out_packet, rx_buffer, 15);

                rx_state = 0;
                return 1;
            }
    }

    return 0;
}

