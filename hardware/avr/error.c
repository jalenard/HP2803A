/**
 * @file error.c
 * @brief
 */

#include <avr/interrupt.h>
#include "hw_hardware_init.h"

void error_fatal(void)
{
    hardware_init();
    cli();
    while (1)
    {
        ;
    }
}
