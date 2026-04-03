/**
 * @file svc_output.c
 * @brief
 */

#define DEBUG_OLD_CODE 0

#if DEBUG_OLD_CODE
#include <stdint.h>
#include <stdio.h>
#include "svc_output.h"
#include "svc_temperature.h"
#include "sys_counters.h"
#include "sys_output.h"
#include "sys_settings_types.h"
#include "core_defs.h"
#include "utils.h"
#endif

#include "svc_output.h"
#include "sys_output.h"



void svc_transmit_results(void)
{
    sys_transmit_results();
}







#if DEBUG_OLD_CODE
static ResolutionOption infer_resolution_from_count(uint32_t count)
{
    ResolutionOption res;

    if (count >= COUNT_MIN_1SEC && count <= COUNT_MAX_1SEC)
    {
        res = RES_SCALE3;
    }
    else if (count >= COUNT_MIN_10SEC && count <= COUNT_MAX_10SEC)
    {
        res = RES_SCALE4;
    }
    else if (count >= COUNT_MIN_100MS && count <= COUNT_MAX_100MS)
    {
        res = RES_SCALE2;
    }
    else
    {
        res = RES_SCALE1;
        count = 0;
    }

    return res;
}

void svc_output_counts_and_temps(void)
{
    const uint32_t * counts = get_counts();
    const float * temps = get_temps();

    if (!counts || !temps)
    {
        return;
    }

    uint8_t res = infer_resolution_from_count(counts[0]);

    char buf[38];
    utils_int_to_text(utils_clamp_u32(counts[0]), 9, &buf[0]);
    utils_int_to_text(utils_clamp_u32(counts[1]), 9, &buf[10]);
    buf[9]   = ',';
    buf[19]  = ',';
    utils_float_to_text(temps[0], 8, res, &buf[20]);
    utils_float_to_text(temps[1], 8, res, &buf[29]);
    buf[28]  = ',';

    sys_output_string(buf);
}
#endif

