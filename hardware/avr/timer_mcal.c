/**
 * @file timer_mcal.c
 * @brief
 */

#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include "input_mcal.h"
#include "input_volatile.h"
#include "timer_mcal.h"

#if 1  // DEBUG: serial feedback
#include <stdio.h>
#include "sys_output.h"
#endif


static uint8_t ring_idx = 0;
static HWKey int0_log[4] =
{
    HW_KEY_NULL, HW_KEY_NULL, HW_KEY_NULL, HW_KEY_NULL
};
static volatile bool press_locked = false;

void timer_mcal_init(void)
{
    /* Timer 0 */
    TCCR0A  = (1 << WGM01);               // CTC mode
    TCCR0B = (1 << CS02) | (1 << CS00);   // Prescaler 1024 (starts timer)
    OCR0A   = 249;                        // 16 ms @ 16 MHz / 1024
    TCNT0   = 0;                          // Reset counter
    TIFR0   = (1 << OCF0A);               // Clear compare flag
    TIMSK0 &= (uint8_t)~(1 << OCIE0A);    // Disable interrupt

    /* Timer 1 */
    TCCR1A  = 0;
    TCCR1B  = (1 << WGM12) | (1 << CS12); // CTC mode & prescaler 256 (starts timer)
    OCR1A   = 62499;                      // 1 s @ 16 MHz / 256
    TCNT1   = 0;                          // Reset counter
    TIFR1   = (1 << OCF1A);               // Clear compare flag
    TIMSK1 &= (uint8_t)~(1 << OCIE1A);    // Disable interrupt

#if 0
    start_timer0();
#endif
}

ISR(TIMER0_COMPA_vect)
{
    /* Freeze push-button value (INT0) */
    uint8_t key = int0_key;

    /* Write key value into log */
    int0_log[ring_idx] = key;

    // Check if last four readings are identical
    bool int0_stable = ((int0_log[0] == int0_log[1]) &&
                        (int0_log[1] == int0_log[2]) &&
                        (int0_log[2] == int0_log[3]));

    if (int0_stable)
    {
        if (key == HW_KEY_NULL)
        {
            isr_key = key;
            press_locked = false;

            stop_timer0();
        }
        else if (press_locked == false)
        {
            isr_key = key;
            press_locked = true;
        }
    }

    /* Advance index modulo 4 */
    ring_idx = (ring_idx + 1) & 0x03;

    /* Consume key value (allows INTO to register new value) */
    int0_key = HW_KEY_NULL;

    /* Enable INT0 */
    if (!(EIMSK & (1 << INT0)))
    {
        EIFR  |= (1 << INTF0);
        EIMSK |= (1 << INT0);
    }
}

ISR(TIMER1_COMPA_vect)
{
    /* no-op */
}

void start_timer0(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCNT0 = 0;                // Reset counter
        TIFR0 = (1 << OCF0A);     // Clear flag
        TIMSK0 |= (1 << OCIE0A);  // Enable interrupt
    }
}

void start_timer1(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TCNT1 = 0;                // Reset counter
        TIFR1 = (1 << OCF1A);     // Clear flag
        TIMSK1 |= (1 << OCIE1A);  // Enable interrupt
    }
}

void stop_timer0(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TIMSK0 &= (uint8_t)~(1 << OCIE0A);  // Disable interrupt
        TIFR0  =  (1 << OCF0A);             // Clear flag
    }
}

void stop_timer1(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        TIMSK1 &= (uint8_t)~(1 << OCIE1A);  // Disable interrupt
        TIFR1  |=  (1 << OCF1A);            // Clear flag
    }
}

