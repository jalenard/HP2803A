/**
 * @file cpu_interrupt.c
 * @brief
 */

#include <avr/interrupt.h>
#include "cpu_interrupt.h"

void cpu_interrupts_enable(void)
{
    sei();
}

void cpu_interrupts_disable(void)
{
    cli();
}

