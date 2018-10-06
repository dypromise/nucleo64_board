#ifndef PERIPHERAL_KB_UART_H_
#define PERIPHERAL_KB_UART_H_

#include "gpio.h"


#ifdef STM32
typedef enum _uart_device_t {
    MM_USART1 = 1,
    MM_USART2,
    MM_USART3,
    MM_UART4,
} uart_device_t;

typedef struct _uart_t {
    UART_HandleTypeDef *handler;
} uart_t;

typedef struct _uart_init_t {
    uart_device_t device;
    uint32_t baud_rate;
    gpio_port_t tx_port;
    gpio_pin_t tx_pin;
    gpio_pull_t tx_pull;
    gpio_port_t rx_port;
    gpio_pin_t rx_pin;
    gpio_pull_t rx_pull;
} uart_init_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

int uart_init(uart_t *obj, uart_init_t *setting);

int uart_send(uart_t *obj, uint8_t *buffer, uint16_t size, uint32_t timeout);

int uart_send_str(uart_t *obj, char *str, uint32_t timeout);

int uart_receive(uart_t *obj, uint8_t *buffer, uint16_t size, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_KB_UART_H_ */
