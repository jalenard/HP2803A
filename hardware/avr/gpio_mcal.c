/**
 * @file gpio_mcal.c
 * @brief
 */

#include <stdint.h>
#include <avr/io.h>
#include "sys_settings_types.h"
#include "gpio_defs.h"
#include "gpio_mcal.h"
#include "gpio_utils.h"

void gpio_mcal_init(void)
{
    uint8_t mask;
    uint8_t value;

    // Data direction registers   Input      Output
    DDRA = 0xFF;  // 0b11111111   -          PA7:0
    DDRB = 0x1F;  // 0b00011111   PB7:5      PB4:0
    DDRC = 0x00;  // 0b00000000   PC7:0      -
    DDRD = 0x42;  // 0b01000010   PD7,5:2,0  PD1,6

    // Port A  PA7:0 = 0
    gpio_write_masked(0xFF, 0x00, GPIO_PORTA);

    // Port B
    //      !EXEC/!WR    TB-A         TB-B         RCLK         !START/!FL
    mask  = (uint8_t)((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4));
    value = (uint8_t)((1 << PB0) | (0 << PB1) | (1 << PB2) | (0 << PB3) | (1 << PB4));
    gpio_write_masked(mask, value, GPIO_PORTB);

    // Port C (input)

    // Port D
    mask  = (uint8_t)((1 << PD1_TXD) | (1 << PD6_SOCKET));
    value = (uint8_t)((0 << PD1_TXD) | (1 << PD6_SOCKET));
    gpio_write_masked(mask, value, GPIO_PORTD);

    // Pull-up resistors
    PORTD |= (uint8_t)((1 << PD4_ROTB) | (1 << PD5_ROTA));
}

/**
 * @brief Configure the timebase selection lines PB1 (TB-A) and PB2 (TB-B).
 *
 * Updates PB1 and PB2 on PORTB according to the requested timebase mode.
 * If the given timebase value is not recognized, the function safely does nothing.
 *
 * @param timebase  One of:
 *                  - TIMEBASE_MSEC100 → 10 Hz (0.1 s)
 *                  - TIMEBASE_SEC1    → 1 Hz  (1 s)
 *                  - TIMEBASE_SEC10   → 0.1 Hz (10 s)
 */
void gpio_set_timebase(TimebaseOption timebase)
{
    const uint8_t mask = (uint8_t)((1 << PB1_TBA) | (1 << PB2_TBB));
    uint8_t value = 0;

    switch (timebase)
    {
    case TIMEBASE_100MS:
        value = (1 << PB1_TBA);
        break;

    case TIMEBASE_1SEC:
        value = (1 << PB2_TBB);
        break;

    case TIMEBASE_10SEC:
        value = (1 << PB1_TBA) | (1 << PB2_TBB);
        break;

    default:
        return;
    }

    gpio_write_masked(mask, value, GPIO_PORTB);
}

TimebaseOption gpio_get_timebase(void)
{
    const uint8_t mask = (uint8_t)((1 << PB1_TBA) | (1 << PB2_TBB));
    uint8_t value = PORTB & mask;

    switch (value)
    {
    case (1 << PB1_TBA):
        return TIMEBASE_100MS;
    case (1 << PB2_TBB):
        return TIMEBASE_1SEC;
    case ((1 << PB1_TBA) | (1 << PB2_TBB)):
        return TIMEBASE_10SEC;
    }

    return TIMEBASE_NA;
}

