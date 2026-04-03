/**
 * @file hw_input.c
 * @brief
 */

#include "hw_input.h"
#include "stdin.h"
#if 1
#include <stdio.h>
#endif

void hw_input_init(void)
{
    stdin_init();
}

HWKey hw_input_poll_key(void)
{
    return (HWKey)stdin_poll_key();
}

