#include "common.h"
#include "terminal.h"
#include "uart.h"

char terminal_tx_buffer[160];
char terminal_rx_buffer[160];

int terminal_init(void) {
    uart_tx_pin((uart_device_t)2, TERMINAL_TX_PORT, TERMINAL_TX_PIN, NOPULL);
    uart_rx_pin((uart_device_t)2, TERMINAL_RX_PORT, TERMINAL_RX_PIN, NOPULL);
    return uart_init((uart_device_t)2, TERMINAL_BAUD_RATE);
}

int terminal_puts(char *str) {
    return uart_send_str(TERMINAL_UART, str, TIMEOUT_MAX);
}

char *terminal_gets(char *str) {
    int result = uart_receive(TERMINAL_UART, (uint8_t *) str, 80,
                              TIMEOUT_MAX); // FIXME: more nice gets
    if (result != MM_OK) {
        return NULL;
    }
    return str;
}
