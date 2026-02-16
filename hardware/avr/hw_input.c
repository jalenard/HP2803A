
#include <stdio.h>
#include "hw_input.h"
#include "input_mcal.h"

void hw_input_init(void)
{
    input_mcal_init();
}

HWKey hw_input_poll_key(void)
{
    input_mcal_enable();
    return (HWKey)input_mcal_poll_key();
}

