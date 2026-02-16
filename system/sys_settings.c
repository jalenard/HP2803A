/**
 * @file sys_settings.c
 * @brief Default values and management functions for system settings.
 */

#include <stdbool.h>
#include <stdio.h>
#include "svc_settings.h"   // TODO: violation of layering principles
#include "sys_settings.h"
#include "sys_settings_apply.h"
#include "sys_settings_types.h"
#include "hw_store.h"

static const Settings default_settings =
{
    .display1_option   = DISPLAY_T1,
    .display2_option   = DISPLAY_T2,
    .units_option      = UNITS_CELSIUS,
    .resolution_option = RES_SCALE3,
    .brightness_value  = 0x66,
    .timebase_option   = TIMEBASE_1SEC,
    .sockets_option    = SOCKETS_FRONT,
    .probe_a_option    = PROBE_IDX0,
    .probe_b_option    = PROBE_IDX1,
};

static Settings current_settings;
static Settings requested_settings;
static bool settings_changes_pending = false;

void sys_settings_init(void)
{
    bool sys_settings_init_success = hw_store_settings_init(&current_settings);
    if (!sys_settings_init_success)
    {
        use_default_settings();
    }
    requested_settings = current_settings;
}

void save_current_settings(void)
{
    hw_store_write_settings(&current_settings);
}

void discard_current_settings(void)
{
    hw_store_read_settings(&requested_settings);
    sys_settings_mark_changed();
}

void use_default_settings(void)
{
    requested_settings = default_settings;
    sys_settings_mark_changed();
}

const Settings * get_current_settings(void)
{
    return &current_settings;  // Read-only pointer
}

Settings * get_current_settings_mutable(void)
{
    return &current_settings;  // Writable pointer
}

const Settings * get_requested_settings(void)
{
    return &requested_settings;  // Read-only pointer
}

Settings * get_requested_settings_mutable(void)
{
    return &requested_settings;  // Writable pointer
}

BrightnessValue sys_settings_get_brightness(void)
{
    return current_settings.brightness_value;
}

void sys_settings_set_brightness(BrightnessValue value)
{
    current_settings.brightness_value = value;
}

void sys_settings_mark_changed(void)
{
    settings_changes_pending = true;
}

bool sys_settings_changes_pending(void)
{
    return settings_changes_pending;
}

void sys_settings_clear_pending(void)
{
    settings_changes_pending = false;
}

