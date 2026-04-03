/**
 * @file hw_display.c
 * @brief
 */

#define DISABLE_CHAR_AND_ROW_UPDATE 1

#include <stdint.h>
#include "hw_display.h"
#include "stdout.h"

void hw_display_init(void)
{
    stdout_init();
}

void hw_display_clear(void)
{
    stdout_clear();
}

void hw_display_update_char(uint8_t row, uint8_t col, char view_buf_char)
{
#if DISABLE_CHAR_AND_ROW_UPDATE
    (void)row;
    (void)col;
    (void)view_buf_char;
#else
    stdout_update_char(row, col, view_buf_char);
#endif
}

void hw_display_update_row(uint8_t row, const char * view_buf_row)
{
#if DISABLE_CHAR_AND_ROW_UPDATE
    (void)row;
    (void)view_buf_row;
#else
    stdout_update_row(row, view_buf_row);
#endif
}

void hw_display_update(const char view_buf_in[][DISPLAY_COLS])
{
    stdout_update(view_buf_in);
}

void hw_display_set_brightness(uint8_t row, uint8_t brightness)
{
    /* no-op */
    (void)row;
    (void)brightness;
}

void hw_display_set_blink(uint8_t row, uint8_t brightness, uint8_t blink)
{
    /* no-op */
    (void)row;
    (void)brightness;
    (void)blink;
}

