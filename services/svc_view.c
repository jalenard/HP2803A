/**
 * @file svc_view.c
 * @brief
 */

#include <stdbool.h>
#include "svc_display.h"
#include "svc_sample.h"
#include "svc_view.h"
#include "cor_menu_nav.h"

static DisplayMode current_mode = DISPLAY_MODE_NULL;
static bool display_update_requested = false;

void svc_view_request_update(DisplayMode display_mode)
{
    display_update_requested = true;
    current_mode = display_mode;

    if (display_mode == DISPLAY_MODE_BASE)
    {
        svc_render_base_view();
    }
    else if (display_mode == DISPLAY_MODE_MENU)
    {
        svc_render_fixed_menu_view();
    }
    else if (display_mode == DISPLAY_MODE_BRT)
    {
        svc_render_fixed_menu_view();
        svc_render_dynamic_brt_view();
    }
}

void svc_view_update_if_needed(void)
{
    if (display_update_requested == false)
    {
        return;
    }

    NodeID cursor_node_id = menu_nav_get_cursor_node_id();
    if (current_mode == DISPLAY_MODE_BASE && cursor_node_id != NODE_BASE)
    {
        return;
    }

    if (current_mode == DISPLAY_MODE_MENU && cursor_node_id == NODE_BASE)
    {
        return;
    }

    svc_display_update();
    display_update_requested = false;
    current_mode = DISPLAY_MODE_NULL;
}

