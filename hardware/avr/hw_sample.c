/**
 * @file hw_sample.c
 * @brief
 */

#include "sys_settings_types.h"
#include "hw_sample.h"
#include "gpio_mcal.h"

void set_sample_time(TimebaseOption timebase_option)
{
    gpio_set_timebase(timebase_option);
}

TimebaseOption get_sample_time(void)
{
    return gpio_get_timebase();
}

