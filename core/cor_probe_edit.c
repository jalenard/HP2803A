/* probe_edit.c */

#define DEBUG_DEBUG_OLD_CODE 0

#include <limits.h>
#include <stdio.h>
#include "cor_probe_edit.h"
#include "cor_probe_db.h"
#include "svc_input.h"
#include "cor_core_types.h"
#include "utils.h"
#include "cor_view.h"

static ProbeTableContext ctx_core;
static ProbeTableContext ctx_cf;

MenuFlow probe_set_core_idx(void)
{
    int probe_idx = ctx_core.probe_idx;
    char buf[VIEW_COLS + 1];

    while (1)
    {
        snprintf(buf, sizeof(buf), "Probe %1d ", probe_idx);
        view_set_row(2, buf);

#if DEBUG_OLD_CODE
        CalibrationEntry *entry = &current_entry_core[probe_idx];
#else
        const CalibrationEntry * entry = probe_db_get_core_entry(probe_idx);
#endif
        uint16_t serial_num = entry->serial_num;
        snprintf(buf, sizeof(buf), "%-8d", serial_num);
        view_set_row(3, buf);

        view_update();

        UIKey key_input = svc_input_poll_key();
        switch (key_input)
        {
        case UI_KEY_SELECT:
            ctx_core.probe_idx = probe_idx;
            return MENU_FLOW_SELECT;

        case UI_KEY_BACK:
            ctx_core.probe_idx = 0;
            return MENU_FLOW_BACK;

        case UI_KEY_CANCEL:
            ctx_core.probe_idx = 0;
            return MENU_FLOW_CANCEL;

        case UI_KEY_ROTA:
            if (probe_idx < NUM_PROBES - 1)
            {
                probe_idx++;
            }
            break;

        case UI_KEY_ROTB:
            if (probe_idx > 0)
            {
                probe_idx--;
            }
            break;

        default:
            break;
        }
    }
    return MENU_FLOW_CANCEL;
}

MenuFlow probe_set_core_param(void)
{
    char buf[VIEW_COLS + 1];
    const char* const core_param_label[NUM_PARAMS_CORE] =
    {
        "S/N     ",
        "Slope   ",
        "CFZ     "
    };

    const int probe_idx = ctx_core.probe_idx;
    int param_idx = ctx_core.param_idx;
#if DEBUG_OLD_CODE
    CalibrationEntry* entry = &current_entry_core[probe_idx];
#else
    const CalibrationEntry * entry = probe_db_get_core_entry(probe_idx);
#endif

    int value = 0;
    while (1)
    {
        snprintf(buf, sizeof(buf), "%-8s", core_param_label[param_idx]);
        view_set_row(2, buf);

        switch (param_idx)
        {
        case 0:
            value = (int)(entry->serial_num);
            break;
        case 1:
            value = (int)(entry->slope);
            break;
        case 2:
            value = entry->cfz;
            break;
        }

        snprintf(buf, sizeof(buf), "%*hd", VIEW_COLS, value);
        buf[VIEW_COLS] = '\0';
        view_set_row(3, buf);

        view_update();

        UIKey key_input = svc_input_poll_key();
        switch (key_input)
        {
        case UI_KEY_SELECT:
            ctx_core.param_idx = param_idx;
            ctx_core.value = value;
            return MENU_FLOW_SELECT;

        case UI_KEY_BACK:
            ctx_core.param_idx = 0;
            return MENU_FLOW_BACK;

        case UI_KEY_CANCEL:
            ctx_core.param_idx = 0;
            return MENU_FLOW_CANCEL;

        case UI_KEY_ROTA:
            if (param_idx < NUM_PARAMS_CORE - 1)
            {
                param_idx++;
            }
            break;

        case UI_KEY_ROTB:
            if (param_idx > 0)
            {
                param_idx--;
            }
            break;

        default:
            break;
        }
    }
    return MENU_FLOW_CANCEL;
}

MenuFlow probe_set_core_value(void)
{
    int start_value = ctx_core.value;

    int value = utils_clamp_int(ui_input_edit_number(start_value));
    if (value == INT_MIN)
    {
        return MENU_FLOW_BACK;
    }
    ctx_core.value = value;

    int probe_idx = ctx_core.probe_idx;
    int param_idx = ctx_core.param_idx;

#if DEBUG_OLD_CODE
    CalibrationEntry *entry = &current_entry_core[probe_idx];
#else
    CalibrationEntry * entry = probe_db_get_core_entry_mutable(probe_idx);
#endif
    switch (param_idx)
    {
    case 0:
        entry->serial_num = utils_clamp_u16(value);
        break;
    case 1:
        entry->slope      = utils_clamp_u16(value);
        break;
    case 2:
        entry->cfz        = utils_clamp_i16(value);
        break;
    }
    return MENU_FLOW_BACK;
}

MenuFlow probe_set_cf_idx(void)
{
    int probe_idx = ctx_cf.probe_idx;
    char buf[VIEW_COLS + 1];

    while (1)
    {
        snprintf(buf, sizeof(buf), "Probe %1d ", probe_idx);
        view_set_row(2, buf);

        // Use serial number from core table to select probe (for cf editing)
#if DEBUG_OLD_CODE
        CalibrationEntry *entry = &current_entry_core[probe_idx];
#else
        CalibrationEntry * entry = probe_db_get_core_entry_mutable(probe_idx);
#endif
        snprintf(buf, sizeof(buf), "%-8d", entry->serial_num);
        view_set_row(3, buf);

        view_update();

        UIKey key_input = svc_input_poll_key();
        switch (key_input)
        {
        case UI_KEY_SELECT:
            ctx_cf.probe_idx = probe_idx;
            return MENU_FLOW_SELECT;

        case UI_KEY_BACK:
            ctx_cf.probe_idx = 0;
            return MENU_FLOW_BACK;

        case UI_KEY_CANCEL:
            ctx_cf.probe_idx = 0;
            return MENU_FLOW_CANCEL;

        case UI_KEY_ROTA:
            if (probe_idx < NUM_PROBES - 1)
            {
                probe_idx++;
            }
            break;

        case UI_KEY_ROTB:
            if (probe_idx > 0)
            {
                probe_idx--;
            }
            break;

        default:
            break;
        }
    }
    return MENU_FLOW_CANCEL;
}

MenuFlow probe_set_cf_param(void)
{
    char buf[VIEW_COLS + 1];

    const int probe_idx = ctx_cf.probe_idx;
    int param_idx = ctx_cf.param_idx;
#if DEBUG_OLD_CODE
    CorrectionFactorEntry* entry = &current_entry_cf[probe_idx];
#else
    const CorrectionFactorEntry * entry = probe_db_get_cf_entry(probe_idx);
#endif

    while (1)
    {
        int16_t temp_c = utils_clamp_i16(-80 + (param_idx * 10));
        int n = snprintf(buf, sizeof(buf), "%3hd C", temp_c);
        for (int i = n; i < VIEW_COLS; ++i)
        {
            buf[i] = ' ';
        }
        buf[VIEW_COLS] = '\0';
        view_set_row(2, buf);

        snprintf(buf, sizeof(buf), "%*hd", VIEW_COLS, entry->cf[param_idx]);
        buf[VIEW_COLS] = '\0';
        view_set_row(3, buf);

        view_update();

        UIKey key_input = svc_input_poll_key();
        switch (key_input)
        {
        case UI_KEY_SELECT:
            ctx_cf.param_idx = param_idx;
            ctx_cf.value = entry->cf[param_idx];
            return MENU_FLOW_SELECT;

        case UI_KEY_BACK:
            ctx_cf.param_idx = 0;
            return MENU_FLOW_BACK;

        case UI_KEY_CANCEL:
            ctx_cf.param_idx = 0;
            return MENU_FLOW_CANCEL;

        case UI_KEY_ROTA:
            if (param_idx < NUM_PARAMS_CF - 1)
            {
                param_idx++;
            }
            break;

        case UI_KEY_ROTB:
            if (param_idx > 0)
            {
                param_idx--;
            }
            break;

        default:
            break;
        }
    }
    return MENU_FLOW_CANCEL;
}

MenuFlow probe_set_cf_value(void)
{
    int start_value = ctx_cf.value;

    int value = utils_clamp_int(ui_input_edit_number(start_value));
    if (value == INT_MIN)
    {
        return MENU_FLOW_BACK;
    }
    ctx_cf.value = value;

    int probe_idx = ctx_cf.probe_idx;
    int param_idx = ctx_cf.param_idx;

#if DEBUG_OLD_CODE
    CorrectionFactorEntry* entry = &current_entry_cf[probe_idx];
#else
    CorrectionFactorEntry * entry = probe_db_get_cf_entry_mutable(probe_idx);
#endif
    entry->cf[param_idx] = utils_clamp_i8(value);

    return MENU_FLOW_BACK;
}

