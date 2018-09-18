#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "kernel/uart.h"

void mp_hal_stdout_tx_strn(const char *str, size_t len)
{
    while (len--)
        raw_uart_putchar(*str++);
}

void mp_hal_stdout_tx_str(const char *str) {
    mp_hal_stdout_tx_strn(str, strlen(str));
}

// Efficiently convert "\n" to "\r\n"
void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len)
{
    const char *last = str;
    while (len--) {
        if (*str == '\n') {
            if (str > last) {
                mp_hal_stdout_tx_strn(last, str - last);
            }
            mp_hal_stdout_tx_strn("\r\n", 2);
            ++str;
            last = str;
        } else {
            ++str;
        }
    }
    if (str > last) {
        mp_hal_stdout_tx_strn(last, str - last);
    }
}

int mp_hal_stdin_rx_chr(void)
{
    return raw_uart_getchar();
}
