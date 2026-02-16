/**
 * @file svc_sample.c
 * @brief
 */

#include "svc_display.h"
#include "svc_sample.h"
#include "svc_settings.h"
#include "sys_counters.h"
#include "sys_settings.h"
#include "sys_settings_apply.h"
#include "cor_render_views.h"

void svc_render_base_view(void)
{
    render_base_view();
}

void svc_render_fixed_menu_view(void)
{
    render_fixed_menu_view();
}

void svc_render_dynamic_brt_view(void)
{
    const Settings * curr_settings = get_current_settings();
    const uint8_t brt_value = curr_settings->brightness_value;

    render_dynamic_brt_view(brt_value);
}

void svc_sample_restart_cycle(void)
{
    sys_restart_counters();
}

