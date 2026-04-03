/**
 * @file svc_settings.c
 * @brief
 */

#include <stdbool.h>
#include "svc_settings.h"
#include "sys_settings.h"
#include "sys_settings_apply.h"
#include "sys_settings_types.h"

void svc_settings_init(void)
{
    sys_settings_init();
}

void svc_settings_apply(void)
{
    sys_settings_apply();
}

void svc_settings_handle_pending(void)
{
    bool changes_requested = sys_settings_changes_pending();
    if (!changes_requested)
    {
        return;
    }

    sys_settings_handle_pending();

    sys_settings_clear_pending();
}

