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

extern NodeID cursor_node_id;

void svc_render_base_view(void)
{
    render_base_view();
}

void svc_render_fixed_menu_view(void)
{
    render_fixed_menu_view();
}

void svc_sample_restart_cycle(void)
{
    sys_restart_counters();
}

