/* utils.c */

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svc_display.h"
#include "utils.h"
#include "cor_view.h"

uint8_t utils_get_nibble(uint8_t value, Nibble which)
{
    return (uint8_t)((which == NIBBLE_HIGH)
                     ? ((value >> 4) & 0x0F)
                     : (value & 0x0F));
}

uint8_t utils_set_nibble(uint8_t value, Nibble which, uint8_t nibble)
{
    nibble &= 0x0F;
    return (uint8_t)((which == NIBBLE_HIGH)
                     ? ((value & 0x0F) | (nibble << 4))
                     : ((value & 0xF0) | nibble));
}

uint8_t utils_increment_nibble(uint8_t value, Nibble which)
{
    uint8_t n = utils_get_nibble(value, which);
    return (n < 0xF) ? utils_set_nibble(value, which, ++n) : value;
}

uint8_t utils_decrement_nibble(uint8_t value, Nibble which)
{
    uint8_t n = utils_get_nibble(value, which);
    return (n > 0) ? utils_set_nibble(value, which, --n) : value;
}

int utils_clamp_int(int32_t value)
{
    if (value < INT_MIN)
    {
        return INT_MIN;
    }
    if (value > INT_MAX)
    {
        return INT_MAX;
    }
    return (int)value;
}

int32_t utils_clamp_u32(uint32_t value)
{
    if (value > INT32_MAX)
    {
        return INT32_MAX;
    }
    return (int32_t)value;
}

uint16_t utils_clamp_u16(int value)
{
    if (value < 0)
    {
        return 0;
    }
    if (value > (int)UINT16_MAX)
    {
        return UINT16_MAX;
    }
    return (uint16_t)value;
}

int16_t utils_clamp_i16(int value)
{
    if (value < INT16_MIN)
    {
        return INT16_MIN;
    }
    if (value > INT16_MAX)
    {
        return INT16_MAX;
    }
    return (int16_t)value;
}

int8_t utils_clamp_i8(int value)
{
    if (value < INT8_MIN)
    {
        return INT8_MIN;
    }
    if (value > INT8_MAX)
    {
        return INT8_MAX;
    }
    return (int8_t)value;
}

/* TODO: remove dependency on higher layer */
void int_to_text(char *buf, uint8_t value)
{
    char temp[VIEW_COLS + 1];

    snprintf(temp, sizeof(temp), "%u", value);

    memset(buf, ' ', VIEW_COLS);
    buf[VIEW_COLS] = '\0';

    size_t len = strlen(temp);
    if (len > VIEW_COLS)
    {
        memcpy(buf, temp + (len - VIEW_COLS), VIEW_COLS);
    }
    else
    {
        memcpy(buf + (VIEW_COLS - len), temp, len);
    }
}

int32_t utils_digits_to_int(const uint8_t digits[], uint8_t length, Sign sign)
{
    int32_t result = 0;
    int32_t multiplier = 1;

    for (size_t i = 0; i < length; i++)
    {
        int32_t add = digits[i] * multiplier;

        if (result > (INT32_MAX - add))
        {
            fprintf(stderr, "Number overflow detected!\n");
            return (sign == POSITIVE) ? INT32_MAX : INT32_MIN;
        }

        result += add;
        multiplier *= 10;
    }

    return sign * result;
}

void utils_int_to_digits(int32_t value, uint8_t digits[], Sign *sign)
{
    int max_digits = 5;
    if (value < 0)
    {
        *sign = NEGATIVE;
        value = -value;
    }
    else
    {
        *sign = POSITIVE;
    }

    for (int i = 0; i < max_digits; i++)
    {
        digits[i] = (uint8_t)(value % 10);
        value /= 10;
    }
}

void utils_float_to_text(const float fnum, const uint8_t fieldwidth, const uint8_t res, char* txt)
{
    for (int i = 0; i < fieldwidth; ++i)
    {
        txt[i] = ' ';    // Clear text field
    }
    float f = fabsf(fnum);                              // Work with positive number

    // Extract integer containing integer and required fractional parts
    for (int i = 0; i < res; ++i)
    {
        f = f * 10;
    }
    uint32_t n = (uint32_t)(f + 0.5);  // Round off to nearest integer

    // Strip off integers and convert to text
    int pos = fieldwidth - 1;
    while (pos >= 0 && (n > 0 || pos >= fieldwidth - res - 2))
    {
        if (pos == fieldwidth - res - 1)
        {
            txt[pos] = '.';
        }
        else
        {
            txt[pos] = (char)('0' + (n % 10));
            n = n / 10;
        }
        pos--;
    }

    // Minus sign
    if (fnum < 0.0f)
    {
        if (pos < 0)
        {
            pos = 0;
        }
        txt[pos] = '-';
    }

    txt[fieldwidth] = '\0'; // Null-terminate
}

void utils_int_to_text(const int32_t num, const uint8_t fieldwidth, char* txt)
{
    for (int i = 0; i < fieldwidth; ++i)
    {
        txt[i] = ' ';    // Initialize text field
    }
    uint32_t n = (uint32_t)labs(num);

    int pos = fieldwidth - 1;
    while (pos >= 0 && (n > 0 || pos == fieldwidth - 1))
    {
        txt[pos] = (char)('0' + (n % 10));
        n = n / 10;
        pos--;
    }

    // Minus sign
    if (num < 0)
    {
        if (pos > 0)
        {
            txt[pos] = '-';
        }
        else
        {
            txt[0]   = '-';
        }
    }

    txt[fieldwidth] = '\0'; // Null-terminate
}

