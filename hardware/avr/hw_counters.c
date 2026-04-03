/**
 * @file hardware/avr/hw_counters.c
 * @brief
 */

#include <stdbool.h>
#include <stdint.h>
#include "counters_mcal.h"
#include "hw_counters.h"

bool hw_counters_ready(void)
{
    return counters_gate_closed();
}

void hw_read_counters(uint32_t counts_out[2])
{
    counters_read(counts_out);
}

void hw_restart_counters(void)
{
    counters_restart();
}

