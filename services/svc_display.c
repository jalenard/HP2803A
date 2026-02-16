/**
 * @file svc_display.c
 * @brief Display service built on top of view_buf.
 */

#include <stdio.h>
#include <string.h>
#include "cor_menu_nav.h"
#include "cor_menu_tree.h"
#include "svc_display.h"
#include "cor_view.h"

void svc_display_init(void)
{
    view_clear();
    view_update();
}

void svc_display_blank(void)
{
    view_clear();
}

void svc_display_set_text(uint8_t row, uint8_t col, const char *txt)
{
    if (row >= VIEW_ROWS || col >= VIEW_COLS || txt == NULL)
    {
        return;
    }

    char temp[VIEW_COLS + 1];
    snprintf(temp, sizeof(temp), "%-*s", (int)(VIEW_COLS - col), txt);

    // Start with current row contents
    char line[VIEW_COLS + 1];
    strncpy(line, view_get_row(row), VIEW_COLS);
    line[VIEW_COLS] = '\0';

    size_t copy_len = (size_t)(VIEW_COLS - col);
    memcpy(&line[col], temp, copy_len);
    view_set_row(row, line);
}

void svc_display_show_startup(void)
{
    view_clear();
    view_set_row(0, "HP2803A ");
    view_set_row(1, "Start-up");
    view_update();
}

void svc_display_update(void)
{
    view_update();
}

const char * svc_display_row(uint8_t row)
{
    return view_get_row(row);
}

