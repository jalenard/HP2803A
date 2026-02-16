/**
 * @file hw_hardware_init.c
 * @brief
 */

#include "cpu_interrupt.h"
#include "gpio_mcal.h"
#include "hw_hardware_init.h"
#include "timer_mcal.h"
#include "usart_mcal.h"
#include "hw_display.h"
#include "input_mcal.h"

void hardware_init(void)
{
    gpio_mcal_init();
    timer_mcal_init();
    usart_mcal_init();
    input_mcal_init();
    hw_display_init();
    cpu_interrupts_enable();
}

