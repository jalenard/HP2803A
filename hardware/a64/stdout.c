/**
 * @file stdout.c
 * @brief A64 "stdout" backend for simulated LED display output.
 */

#include <stdio.h>
#include <string.h>
#include "hw_display.h"
#include "stdout.h"

#define DEBUG 1

void stdout_init(void)
{
    fputs("\033[2J\033[H", stdout);
    fflush(stdout);
}

void stdout_clear(void)
{
#if (DEBUG)
    /* no-op */
#else
    /* Clear screen \033[2J and move cursor home \033[H */
    fputs("\033[2J\033[H", stdout);
#endif
    fflush(stdout);
}

void stdout_update_char(int row, int col, char view_buf_char)
{
    /* TODO: Code if required */
    (void)row;
    (void)col;
    (void)view_buf_char;
}

#if DEBUG
void stdout_update_row(int row, const char *view_buf_row)
{
    static char line_buf[(DISPLAY_COLS + 2) * DISPLAY_ROWS + 1]; // room for 4 [rows]
    static int rows_received = 0;

    if (row < 0 || row >= DISPLAY_ROWS || view_buf_row == NULL)
    {
        return;
    }

    /* Copy row into the composite output buffer */
    char tmp[DISPLAY_COLS + 1];
    memcpy(tmp, view_buf_row, DISPLAY_COLS);
    tmp[DISPLAY_COLS] = '\0';

    /* Format the bracketed segment for this row */
    int offset = 0;
    for (int i = 0; i < row; ++i)
    {
        offset += DISPLAY_COLS + 2;    // "[...]" per row
    }

    snprintf(line_buf + offset, DISPLAY_COLS + 3, "[%-*.*s]", DISPLAY_COLS, DISPLAY_COLS, tmp);

    rows_received++;

    /* Once all rows are updated, print the full composite line */
    if (rows_received >= DISPLAY_ROWS)
    {
        line_buf[(DISPLAY_COLS + 2) * DISPLAY_ROWS] = '\0';
        printf("%s\n", line_buf);
        fflush(stdout);
        rows_received = 0;
    }
}
#else
void stdout_update_row(int row, const char * view_buf_row)
{
    if (row < 0 || row >= DISPLAY_ROWS || view_buf_row == NULL)
    {
        return;
    }

    /* Prepare a temporary null-terminated copy for safe printing */
    char tmp[DISPLAY_COLS + 1];
    memcpy(tmp, view_buf_row, DISPLAY_COLS);
    tmp[DISPLAY_COLS] = '\0';

    /* Calculate cursor position */
    int line = row / 2;           // 0 for rows 0–1, 1 for rows 2–3
    int column = (row % 2) * (DISPLAY_COLS + 4); // 0 or offset for second column

    printf("\033[%d;%dH[%-*.*s]", line + 1, column + 1, DISPLAY_COLS, DISPLAY_COLS, tmp);
    printf("\033[3;1H");

    fflush(stdout);
}
#endif

void stdout_update(const char view_buf_in[][DISPLAY_COLS])
{
    if (view_buf_in == NULL)
    {
        return;
    }

    stdout_clear();
    for (int r = 0; r < DISPLAY_ROWS; ++r)
    {
        stdout_update_row(r, view_buf_in[r]);
    }

    fflush(stdout);
}

