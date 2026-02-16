/**
 * @file svc_temperature.c
 * @brief
 */

#include <stdint.h>
#include "svc_temperature.h"
#include "sys_counters.h"
#include "cor_temp_compute.h"

static float temp[2];

void update_temp(void)
{
    const uint32_t * count_in = get_counts();
    calc_celsius_from_count(count_in, temp);
}

const float * get_temps(void)
{
    return temp;
}

