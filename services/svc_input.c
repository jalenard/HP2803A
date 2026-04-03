/**
 * @file svc_input.c
 * @brief
 */

#include "svc_display.h"
#include "svc_input.h"
#include "utils.h"
#include "hw_input.h"
#include "cor_view.h"

#define UI_EDIT_MAX_DIGITS  5
#define UI_EDIT_SIGN_POS    0

UIKey svc_input_poll_key(void)
{
    return hw_input_poll_key();
}

static int digit_pos_to_index(int position)
{
    if (position == UI_EDIT_SIGN_POS)
    {
        return -1;
    }
    return UI_EDIT_MAX_DIGITS - position;
}

static void build_value_line(char out[VIEW_COLS + 1],
                             Sign sign,
                             const uint8_t digits[],
                             int8_t position)
{
    char buf[VIEW_COLS + 1];
    int p = 0;

    // Sign
    if (position == UI_EDIT_SIGN_POS)
    {
        buf[p++] = '(';
        buf[p++] = (sign == POSITIVE ? '+' : '-');
        buf[p++] = ')';
    }
    else
    {
        buf[p++] = (sign == POSITIVE ? '+' : '-');
    }

    // Digits
    for (int display_pos = 1; display_pos <= UI_EDIT_MAX_DIGITS; display_pos++)
    {
        int idx = digit_pos_to_index(display_pos);
        int d = digits[idx];

        if (position == display_pos)
        {
            buf[p++] = '(';
            buf[p++] = (char)('0' + d);
            buf[p++] = ')';
        }
        else
        {
            buf[p++] = (char)('0' + d);
        }
    }

    buf[p] = '\0';
    for (int i = 0; i <= VIEW_COLS; ++i)
    {
        out[i] = buf[i];
    }
}

static void render_value_screen(Sign sign, const uint8_t digits[], int8_t position)
{
    char buf[VIEW_COLS + 1];
    build_value_line(buf, sign, digits, position);

    view_clear();
    view_set_row(3, buf);
    view_update();
}

int32_t ui_input_edit_number(int32_t start_value)
{
    while (1)
    {
        uint8_t digits[UI_EDIT_MAX_DIGITS] = {0};
        Sign sign = POSITIVE;
        utils_int_to_digits(start_value, digits, &sign);

        int8_t position = UI_EDIT_SIGN_POS;

        while (position <= UI_EDIT_MAX_DIGITS)
        {
            render_value_screen(sign, digits, position);

            UIKey key = svc_input_poll_key();
            if (key == UI_KEY_SELECT)
            {
                if (position < UI_EDIT_MAX_DIGITS)
                {
                    position++;
                }
                else
                {
                    return utils_digits_to_int(digits, UI_EDIT_MAX_DIGITS, sign);
                }
            }
            else if (key == UI_KEY_BACK && position > UI_EDIT_SIGN_POS)
            {
                position--;
            }
            else if (key == UI_KEY_CANCEL)
            {
                return INT32_MIN;
            }
            else if (key == UI_KEY_ROTA || key == UI_KEY_ROTB)
            {
                if (position == UI_EDIT_SIGN_POS)
                {
                    sign = (sign == POSITIVE ? NEGATIVE : POSITIVE);
                }
                else
                {
                    int idx = digit_pos_to_index(position);
                    uint8_t new_digit = digits[idx];
                    if (key == UI_KEY_ROTA  && new_digit < 9)
                    {
                        new_digit++;
                    }
                    if (key == UI_KEY_ROTB && new_digit > 0)
                    {
                        new_digit--;
                    }
                    digits[idx] = new_digit;
                }
            }
        }
    }
}

