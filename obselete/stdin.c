/**
 * @file stdin.c
 * @brief
 */

#include <stdio.h>
#include "hw_input.h"
#include "stdin.h"

void stdin_init(void)
{
    /* no-op */
}

static char stdin_get_char(void)
{
    int ch = getchar();
    if (ch == EOF)
    {
        return (char)EOF;
    }

    if (ch != '\n')
    {
        int discard;
        while ((discard = getchar()) != '\n' && discard != EOF)
            ; // Discard rest of line
    }

    return (char)ch;
}

HWKey stdin_poll_key(void)
{
    char key = stdin_get_char();

    switch (key)
    {
    case 's':
        return HW_KEY_SELECT;
    case 'b':
        return HW_KEY_BACK;
    case 'c':
        return HW_KEY_CANCEL;
    case 'r':
        return HW_KEY_ROTA;
    case 't':
        return HW_KEY_ROTB;
    default:
        return HW_KEY_INVALID;
    }
}

