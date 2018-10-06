#include "common.h"
#include "terminal.h"

char terminal_tx_buffer[200];
char terminal_rx_buffer[200];
static uart_t terminal;

int terminal_init(void) {
    uart_init_t uart_setting = {
            .device = TERMINAL_UART,
            .baud_rate = TERMINAL_BAUD_RATE,
            .tx_port = TERMINAL_TX_PORT,
            .tx_pin = TERMINAL_TX_PIN,
            .tx_pull = NOPULL,
            .rx_port = TERMINAL_RX_PORT,
            .rx_pin = TERMINAL_RX_PIN,
            .rx_pull = NOPULL
    };
    return uart_init(&terminal, &uart_setting);
}

int terminal_puts(char *str) {
    return uart_send_str(&terminal, str, TIMEOUT_MAX);
}

char *terminal_gets(char *str) {
    int result = uart_receive(&terminal, (uint8_t *) str, 80,
                              TIMEOUT_MAX); // FIXME: more nice gets
    if (result != MM_OK) {
        return NULL;
    }
    return str;
}
