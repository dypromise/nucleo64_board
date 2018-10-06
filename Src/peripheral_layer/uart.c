#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "uart.h"

#ifdef MM_MSG_BASE
#undef MM_MSG_BASE
#define MM_MSG_BASE "UART"
#endif

#ifndef STM32
#error "Please define device! " __FILE__ "\n"
#endif


static int _uart_tx_pin(uart_device_t dev, gpio_port_t port, gpio_pin_t pin,
                        gpio_pull_t pull);

static int _uart_rx_pin(uart_device_t dev, gpio_port_t port, gpio_pin_t pin,
                        gpio_pull_t pull);


/*******************************************************************************
 * API functions
 ******************************************************************************/

int uart_init(uart_t *obj, uart_init_t *setting) {
    /* malloc a handler */
    UART_HandleTypeDef *handler = malloc(sizeof(UART_HandleTypeDef));
    obj->handler = handler;
    switch (setting->device) {
        case MM_USART1:
                    __USART1_CLK_ENABLE();
            handler->Instance = USART1;
            break;
        case MM_USART2:
                    __USART2_CLK_ENABLE();
            handler->Instance = USART2;
            break;
        case MM_USART3:
                    __USART3_CLK_ENABLE();
            handler->Instance = USART3;
            break;
        case MM_UART4:
                    __UART4_CLK_ENABLE();
            handler->Instance = UART4;
            break;
        default:
            return MM_ERROR;
    }
    handler->Init.BaudRate = setting->baud_rate;
    handler->Init.WordLength = UART_WORDLENGTH_8B;
    handler->Init.StopBits = UART_STOPBITS_1;
    handler->Init.Parity = UART_PARITY_NONE;
    handler->Init.Mode = UART_MODE_TX_RX;
    handler->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    handler->Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(handler) != HAL_OK)
        return MM_ERROR;
    if (_uart_rx_pin(setting->device, setting->rx_port, setting->rx_pin,
                     setting->rx_pull) == MM_OK &&
        _uart_tx_pin(setting->device, setting->tx_port, setting->tx_pin,
                     setting->tx_pull) == MM_OK)
        return MM_OK;

    MM_DEBUG_ERROR("UART pin init error!\r\n");
    return MM_ERROR;
}


int uart_send(uart_t *obj, uint8_t *buffer, uint16_t size, uint32_t timeout) {
    int8_t result = HAL_UART_Transmit(obj->handler, buffer, size, timeout);
    return (status_t) result;
}

int uart_send_str(uart_t *obj, char *str, uint32_t timeout) {
    return uart_send(obj, (uint8_t *) str, (uint16_t) strlen(str), timeout);
}

int
uart_receive(uart_t *obj, uint8_t *buffer, uint16_t size, uint32_t timeout) {
    int8_t result = HAL_UART_Receive(obj->handler, buffer, size, 0);
    return (status_t) result;
}


/*******************************************************************************
 * Private functions
 ******************************************************************************/
int
_uart_tx_pin(uart_device_t dev, gpio_port_t port, gpio_pin_t pin,
             gpio_pull_t pull) {
    uint32_t alternate = MM_WRONG_PIN;
    switch (dev) {
        case MM_USART1:
            alternate = GPIO_AF7_USART1;
            break;
        case MM_USART2:
            alternate = GPIO_AF7_USART2;
            break;
        case MM_USART3:
            alternate = GPIO_AF7_USART3;
            break;
        case MM_UART4:
            alternate = GPIO_AF8_UART4;
            break;
        default:
            MM_DEBUG_ERROR("Wrong usart selected!\r\n");
            break;

    }
    if (alternate == MM_WRONG_PIN) {
        return MM_ERROR;
    }

    /* Init GPIO */
    gpio_enable_clk(port);
    gpio_init_t gpio_setting = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = pull,
            .Alternate = alternate,
            .Speed = GPIO_SPEED_LOW
    };
    gpio_init(port, pin, &gpio_setting);
    return MM_OK;
}

int
_uart_rx_pin(uart_device_t dev, gpio_port_t port, gpio_pin_t pin,
             gpio_pull_t pull) {
    uint32_t alternate = MM_WRONG_PIN;
    switch (dev) {
        case MM_USART1:
            alternate = GPIO_AF7_USART1;
            break;
        case MM_USART2:
            alternate = GPIO_AF7_USART2;
            break;
        case MM_USART3:
            alternate = GPIO_AF7_USART3;
            break;
        case MM_UART4:
            alternate = GPIO_AF8_UART4;
            break;
        default:
            MM_DEBUG_ERROR("Wrong usart selected!\r\n");
            break;

    }
    if (alternate == MM_WRONG_PIN) {
        return MM_ERROR;
    }
    gpio_enable_clk(port);
    /* Init GPIO */
    gpio_init_t gpio_setting = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = pull,
            .Alternate = alternate,
            .Speed = GPIO_SPEED_LOW
    };
    gpio_init(port, pin, &gpio_setting);
    return MM_OK;
}
