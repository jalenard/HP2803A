/**
 * @file hw_display.c
 * @brief
 */

#include "gpio_defs.h"
#include "hw_display.h"
#include "led_mcal.h"

void hw_display_init(void)
{
    led_init();
}

void hw_display_clear(void)
{
    led_clear();
}

void hw_display_update_char(uint8_t row, uint8_t col, char view_buf_char)
{
    led_update_char(row, col, view_buf_char);
}

void hw_display_update_row(uint8_t row, const char * view_buf_row)
{
    led_update_row(row, view_buf_row);
}

void hw_display_update(const char view_buf_in[][DISPLAY_COLS])
{
    led_update(view_buf_in);
}

void hw_display_set_brightness(uint8_t row, uint8_t brightness)
{
    led_set_brightness(row, brightness);
}

void hw_display_set_blink(uint8_t row, uint8_t brightness, uint8_t blink)
{
    led_set_blink(row, brightness, blink);
}

