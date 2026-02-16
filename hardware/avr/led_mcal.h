/**
 * @file led_mcal.h
 * @brief
 */

#ifndef LED_H
#define LED_H

#include <stdint.h>

void led_bus_write(uint8_t LedA, uint8_t LedD);
void led_write_flash(uint8_t LedA, uint8_t LedD);
void led_clear(void);
void led_blank(void);
void led_init(void);
void led_update_char(uint8_t row, int col, char view_buf_char);
void led_update_row(uint8_t row, const char * view_buf_in);
void led_update(const char view_buf_in[][DISPLAY_COLS]);
void led_set_brightness(int display, uint8_t brightness);
void led_set_blink(int display, uint8_t brightness, uint8_t blink);

#endif

