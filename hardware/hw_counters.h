/**
 * @file hw_counters.h
 * @brief
 */

#ifndef HW_COUNTERS_H
#define HW_COUNTERS_H

#include <stdbool.h>
#include <stdint.h>

bool hw_counters_ready(void);
void hw_read_counters(uint32_t count_out[2]);
void hw_restart_counters(void);

#endif /* HW_COUNTERS_H */
