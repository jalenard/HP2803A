/**
 * @file cor_data_provider.c
 * @brief
 */

#include "svc_temperature.h"
#include "sys_counters.h"
#include "sys_output.h"
#include "sys_settings.h"
#include "cor_data_provider.h"

const float * cor_get_temps(void)
{
    return get_temps();
}

const uint32_t * cor_get_counts(void)
{
    return get_counts();
}

const Settings * cor_get_current_settings(void)
{
    return get_current_settings();
}

char * cor_get_output_buf_mutable(void)
{
    return get_output_buf_mutable();
}

