#ifndef PERIPHERAL_KB_UART_H_
#define PERIPHERAL_KB_UART_H_

#include "gpio.h"


#ifdef STM32
typedef USART_TypeDef *uart_t;
typedef enum _uart_device_t {
    MM_USART1 = 1,
    MM_USART2,
    MM_USART3,
    MM_UART4,
} uart_device_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

int uart_init(uart_device_t dev, uint32_t baud_rate);

int
uart_tx_pin(uart_device_t dev, gpio_port_t port, gpio_pin_t pin,
            gpio_pull_t pull);

int
uart_rx_pin(uart_device_t dev, gpio_port_t port, gpio_pin_t pin,
            gpio_pull_t pull);

int uart_send(uart_t uart, uint8_t *buffer, uint16_t size, uint32_t timeout);

int uart_send_str(uart_t uart, char *str, uint32_t timeout);

int uart_receive(uart_t uart, uint8_t *buffer, uint16_t size, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_KB_UART_H_ */
