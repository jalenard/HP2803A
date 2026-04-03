/**
 * @file svc_settings_request.h
 * @brief
 */

#include "svc_settings.h"
#include "sys_settings.h"
#include "sys_settings_apply.h"
#include "sys_settings_types.h"

static Settings requested_settings;

void svc_handle_settings_requests(void)
{
    Settings * curr_settings = get_current_settings_mutable();

    TimebaseOption curr_tb = curr_settings->timebase_option;
    TimebaseOption reqd_tb = requested_settings.timebase_option;

    if (reqd_tb != curr_tb)
    {
        sys_settings_apply_timebase(reqd_tb);
    }

    *curr_settings = requested_settings;
}

const Settings * get_requested_settings(void)
{
    return &requested_settings;
}

Settings * get_requested_settings_mutable(void)
{
    return &requested_settings;
}

