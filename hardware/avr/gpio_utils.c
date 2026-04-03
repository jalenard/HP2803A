/**
 * @file gpio_utils.c
 * @brief
 */

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "gpio_defs.h"
#include "gpio_utils.h"
#include "error.h"

void ddrc_set_outputs_protected(uint8_t mask)
{
    if ((PINA & (1 << PA7_ENABLE)) != 0)  // PA7 must be low
    {
        DDRC = 0x00;
        return;
    }

    DDRC = mask;
}

void gpio_write_masked(uint8_t mask, uint8_t value, GpioPort gpio_port)
{
    volatile uint8_t * ddr;
    volatile uint8_t * port;

    switch (gpio_port)
    {
    case GPIO_PORTA:
        ddr  = &DDRA;
        port = &PORTA;
        break;
    case GPIO_PORTB:
        ddr  = &DDRB;
        port = &PORTB;
        break;
    case GPIO_PORTC:
        ddr  = &DDRC;
        port = &PORTC;
        break;
    case GPIO_PORTD:
        ddr  = &DDRD;
        port = &PORTD;
        break;
    default:
        error_fatal();
        return;
    }

    /* Verify mask applies only to configured outputs */
    if ((mask & (uint8_t)~(*ddr)) != 0)
    {
        error_fatal();
        return;
    }

    // Masked write to PORTx
    *port = (uint8_t)((*port & ~mask) | (value & mask));
}

void toggle_START(void)
{
    // Error condition
    if ((PINB & (1 << PB4_nSTART)) == 0)
    {
        error_fatal();    // START should be low (!START high) (default)
    }

    // Toggle !START on PB4
    uint8_t mask  = (1 << PB4_nSTART);
    uint8_t value = (0 << PB4_nSTART);
    gpio_write_masked(mask, value, GPIO_PORTB); // Set START high (!START low)

    _delay_us(1);                               // TODO Specification X ns

    value = (1 << PB4_nSTART);
    gpio_write_masked(mask, value, GPIO_PORTB); // Set START low (!START high)
}

void toggle_WRITE(void)
{
    // Error condition
    if ((PINB & (1 << PB0_nWR)) == 0)
    {
        error_fatal();    // WRITE should be low (!WR high) (default)
    }

    // Toggle !WRITE on PB0
    uint8_t mask  = (1 << PB0_nWR);
    uint8_t value = (0 << PB0_nWR);
    gpio_write_masked(mask, value, GPIO_PORTB); // Set WRITE high (!WRITE low)

    _delay_us(1);                               // Specification 140 ns

    value = (1 << PB0_nWR);
    gpio_write_masked(mask, value, GPIO_PORTB); // Set WRITE low (!WRITE high)
}

