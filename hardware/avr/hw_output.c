/**
 * @file hw_output.c
 * @brief
 */

#include "hw_output.h"
#include "usart_mcal.h"

void hw_output_transmit(uint8_t byte)
{
    serial_transmit(byte);
}

