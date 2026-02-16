/**
 * @file system/sys_counters.c
 * @brief
 */

#include <stdbool.h>
#include <stdint.h>
#include "hw_counters.h"
#include "sys_counters.h"

static uint32_t count[2];

bool sample_period_complete(void)
{
    return hw_counters_ready();
}

void sys_restart_counters(void)
{
    hw_restart_counters();
}

void read_measurement(void)
{
    hw_read_counters(count);
}

const uint32_t * get_counts(void)
{
    return count;
}

