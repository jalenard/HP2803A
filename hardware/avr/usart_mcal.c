/**
 * @file usart_mcal.c
 * @brief
 */

#define USE_2X 1           // Double-speed mode (U2X0=1)
#define BAUD_TOL 3         // 3% error tolerance level

#include <avr/io.h>
#include "error.h"
#include "usart_mcal.h"
#include <util/setbaud.h>  // Must follow BAUD definition (in usart_mcal.h)

void usart_mcal_init(void)
{
    UCSR0B |= (uint8_t)(1 << RXCIE0);  // Enable RX complete interrupt

    // Set baud rate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    // Enable transmitter
    UCSR0B |=  (uint8_t)(1 << TXEN0);
    //UCSR0B &= (uint8_t)~(1 << RXEN0);

    // Set frame format: 8 data bits, no parity, 1 stop bit
    UCSR0B &= (uint8_t)~(1 << UCSZ02);  // USART character size - 0b011=8-bit
    UCSR0C |=  (uint8_t)(1 << UCSZ01);  // USART character size - 0b011=8-bit
    UCSR0C |=  (uint8_t)(1 << UCSZ00);  // USART character size - 0b011=8-bit
    UCSR0C &= (uint8_t)~(1 << UPM01);   // Parity mode 0b00=disabled; 0b10=enabled (even); 0b11=enabled (odd)
    UCSR0C &= (uint8_t)~(1 << UPM00);   // Parity mode 0b00=disabled; 0b10=enabled (even); 0b11=enabled (odd)
    UCSR0C &= (uint8_t)~(1 << USBS0);   // USART stop bit select - 0=1-bit; 1=2-bit
}

void serial_transmit(uint8_t data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (uint8_t)(1 << UDRE0)));  // Buffer can be written when UDREn flag in UCSRnA register is set

    // Put data into buffer
    UDR0 = data;
}

uint8_t serial_receive(void)
{
    // USART receiver error flags
    if (UCSR0A & (uint8_t)(1 << FE0))
    {
        error_fatal();    // Frame error
    }
    if (UCSR0A & (uint8_t)(1 << DOR0))
    {
        error_fatal();    // Data over-run
    }
    if (UCSR0A & (uint8_t)(1 << UPE0))
    {
        error_fatal();    // Parity error
    }

    // Wait for data to be received
    while (!(UCSR0A & (uint8_t)(1 << RXC0)));

    // Return data from buffer
    return UDR0;
}

