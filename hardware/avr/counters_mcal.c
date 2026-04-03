/**
 * @file hardware/avr/counters_mcal.c
 * @brief
 */

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "counters_mcal.h"
#include "error.h"
#include "gpio_defs.h"
#include "gpio_utils.h"

bool counters_gate_closed(void)
{
    return (bool)PD7_GATE_CLOSED;
}

void counters_read(uint32_t * counts_out)
{
    // Error conditions
    if (DDRC != 0x00)
    {
        error_fatal();    // Port C must be input (0x00)
    }
    if ((PINB & (uint8_t)(1 << PB3_RCLK)) != 0)
    {
        error_fatal();    // RCLK should be low
    }
    if ((PINB & (uint8_t)(1 << PB0_nEXEC)) == 0)
    {
        error_fatal();    // EXEC should be low (!EXEC high)
    }

    uint8_t counter_num;
    uint8_t mask;
    uint8_t value;

    // Toggle RCLK
    mask  =  (uint8_t)(1 << PB3_RCLK);
    value =  (uint8_t)(1 << PB3_RCLK);                   // RCLK high
    gpio_write_masked(mask, value, GPIO_PORTB);
    _delay_us(1);                                        // spec. 10 ns
    value =  (uint8_t)(0 << PB3_RCLK);                   // RCLK low
    gpio_write_masked(mask, value, GPIO_PORTB);

    // Read counters 3,2,1,0 into counts_out[0] and 7,6,5,4 into counts_out[1]
    for (int j = 0; j < 2; ++j)
    {
        counts_out[j] = 0;
        for (int i = 3; i >= 0; --i)
        {
            // Select and enable counter
            counter_num = (uint8_t)(i + (j * 4));
            counter_num &= 0x07;                         // Enforce range 0b000 to 0b111
            mask  = (uint8_t)((1 << PA0) | (1 << PA1) | (1 << PA2) | (1 << PA7));
            value = (uint8_t)((counter_num << PA0) | (1 << PA7)); // Counter number in PA0-PA2, PA7 enabled
            gpio_write_masked(mask, value, GPIO_PORTA);

            // Toggle EXEC to output latch value
            mask  = (uint8_t)(1 << PB0_nEXEC);
            value = (uint8_t)(0 << PB0_nEXEC);           // EXEC high (!EXEC low)
            gpio_write_masked(mask, value, GPIO_PORTB);

            // Insert 8-bit counter value into 32-bit counts_out value
            counts_out[j] = (counts_out[j] << 8);        // Shift count 8 bits left
            counts_out[j] += PINC;                       // Add 8-bit counter value

            // Reset EXEC
            value = (uint8_t)(1 << PB0_nEXEC);           // EXEC low (!EXEC high)
            gpio_write_masked(mask, value, GPIO_PORTB);
        }
    }
}

void counters_restart(void)
{
    toggle_START();
    _delay_us(10);             // Allow 10 µs delay in time base
    while (PD7_GATE_CLOSED) {} /* TODO: review this double delay */
}

