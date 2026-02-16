/**
 * @file sys_output.c
 * @brief System-level output abstraction layer.
 */

#define OUTPUT_COLS 38

#include "sys_output.h"
#include "cor_render_views.h"
#include "hw_output.h"

static char output_buf[OUTPUT_COLS];

const char * get_output_buf(void)
{
    return output_buf;
}

char * get_output_buf_mutable(void)
{
    return output_buf;
}


void sys_transmit_results(void)
{
    render_output_view();
    sys_output_string(output_buf);
}

void sys_output_transmit(uint8_t byte)
{
    hw_output_transmit(byte);
}

void sys_output_string(const char * str)
{
    for (int i = 0; str[i] != '\0'; ++i)
    {
        sys_output_transmit((uint8_t)str[i]);
    }
    sys_output_transmit('\n');
}

