/**
 * @file sys_settings_apply.c
 * @brief
 */

#include <stdbool.h>
#include "sys_settings.h"
#include "sys_settings_apply.h"
#include "sys_settings_types.h"
#include "hw_sample.h"

void sys_settings_apply_timebase(TimebaseOption timebase_option)
{
    set_sample_time(timebase_option);
}

void settings_apply_sockets(SocketsOption sockets_option)
{
    (void)sockets_option;
}

void settings_apply_probe_a(ProbeOption probe_a_option)
{
    (void)probe_a_option;
}

void settings_apply_probe_b(ProbeOption probe_b_option)
{
    (void)probe_b_option;
}

void settings_apply_transmit(TransmitOption transmit_option)
{
    (void)transmit_option;
}

/* TODO: Harmonise sys_settings_apply() and sys_handle_settings_requests */
/*       Functions for individual settings => static? */
void sys_settings_apply(void)
{
    const Settings * curr_settings = get_current_settings();
    TimebaseOption curr_timebase = curr_settings->timebase_option;

    sys_settings_apply_timebase(curr_timebase);
}

void sys_settings_handle_pending(void)
{
    Settings * curr_settings = get_current_settings_mutable();
    const Settings * reqd_settings = get_requested_settings();

    TimebaseOption curr_tb = curr_settings->timebase_option;
    TimebaseOption reqd_tb = reqd_settings->timebase_option;

    if (reqd_tb != curr_tb)
    {
        sys_settings_apply_timebase(reqd_tb);
    }

    *curr_settings = *reqd_settings;
}

