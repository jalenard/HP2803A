/**
 * @file stdin.c
 * @brief Non-blocking stdin keyboard driver for A64/Linux.
 *
 * NOTE:
 *   The terminal must be configured externally using commands such as:
 *      stty -icanon -echo
 *
 *   This driver does NOT modify the terminal.
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#include "hw_input.h"
#include "stdin.h"

void stdin_init(void)
{
    /* No terminal configuration. User must configure externally:
     *
     * Before running program:
     * stty -icanon -echo min 0 time 0
     * ./build/prog
     *
     * After running program:
     * stty sane
     */
}

static int stdin_get_char_nonblocking(void)
{
    fd_set rfds;
    struct timeval tv = {0, 0};   // zero timeout → non-blocking

    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);

    int retval = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);

    if (retval <= 0)
    {
        return -1;    // no input
    }

    unsigned char ch;
    ssize_t n = read(STDIN_FILENO, &ch, 1);

    if (n <= 0)
    {
        return -1;
    }

    return ch;
}

HWKey stdin_poll_key(void)
{
    int ch = stdin_get_char_nonblocking();
    if (ch < 0)
    {
        return HW_KEY_NULL;
    }

    switch (ch)
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

