/**
 * @file hardware/a64/hw_counters.c
 * @brief
 */

#include <stdbool.h>
#include <stdint.h>
#include "hw_counters.h"
#include "timer.h"

bool hw_counters_ready(void)
{
    return timer_expired();
}

void hw_read_counters(uint32_t counts_out[2])
{
    timer_sine_wave(counts_out);
}

void hw_restart_counters(void)
{
    timer_restart();
}
