#include <string.h>
#include "common.h"
#include "uart.h"

#ifdef MM_MSG_BASE
#undef MM_MSG_BASE
#define MM_MSG_BASE "UART"
#endif

#if defined(STM32)
static UART_HandleTypeDef uart_1_h_;
static UART_HandleTypeDef uart_2_h_;
static UART_HandleTypeDef uart_3_h_;
static UART_HandleTypeDef uart_4_h_;
static UART_HandleTypeDef uart_5_h_;
static UART_HandleTypeDef uart_6_h_;
#else
#error "Please define device! " __FILE__ "\n"
#endif

int uart_init(uart_device_t dev, uint32_t baud_rate) {
    // select handler
    UART_HandleTypeDef *handler;
    switch (dev) {
        case MM_USART1:
            handler = &uart_1_h_;
                    __USART1_CLK_ENABLE();
            handler->Instance = USART1;
            break;
        case MM_USART2:
            handler = &uart_2_h_;
                    __USART2_CLK_ENABLE();
            handler->Instance = USART2;
            break;
        case MM_USART3:
            handler = &uart_3_h_;
                    __USART3_CLK_ENABLE();
            handler->Instance = USART3;
            break;
        case MM_UART4:
            handler = &uart_4_h_;
                    __UART4_CLK_ENABLE();
            handler->Instance = UART4;
            break;
        default:
            return MM_ERROR;
    }


    handler->Init.BaudRate = baud_rate;
    handler->Init.WordLength = UART_WORDLENGTH_8B;
    handler->Init.StopBits = UART_STOPBITS_1;
    handler->Init.Parity = UART_PARITY_NONE;
    handler->Init.Mode = UART_MODE_TX_RX;
    handler->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    handler->Init.OverSampling = UART_OVERSAMPLING_16;

    int8_t result = HAL_UART_Init(handler);
    return (status_t) result;
}

int
uart_tx_pin(uart_device_t dev, gpio_port_t port, gpio_pin_t pin,
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
    // Init GPIO
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
uart_rx_pin(uart_device_t dev, gpio_port_t port, gpio_pin_t pin,
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
    // Init GPIO
    gpio_init_t gpio_setting = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = pull,
            .Alternate = alternate,
            .Speed = GPIO_SPEED_LOW
    };
    gpio_init(port, pin, &gpio_setting);
    return MM_OK;
}

int uart_send(uart_t uart, uint8_t *buffer, uint16_t size, uint32_t timeout) {
    // select handler
    UART_HandleTypeDef *handler;
    if (uart == USART1) {
        handler = &uart_1_h_;
    } else if (uart == USART2) {
        handler = &uart_2_h_;
    } else if (uart == USART3) {
        handler = &uart_3_h_;
    } else if (uart == UART4) {
        handler = &uart_4_h_;
    } else if (uart == UART5) {
        handler = &uart_5_h_;
    } else if (uart == USART6) {
        handler = &uart_6_h_;
    } else {
        return MM_ERROR;
    }
    int8_t result = HAL_UART_Transmit(handler, buffer, size, timeout);
    return (status_t) result;
}

int uart_send_str(uart_t uart, char *str, uint32_t timeout) {
    return uart_send(uart, (uint8_t *) str, (uint16_t) strlen(str), timeout);
}

int
uart_receive(uart_t uart, uint8_t *buffer, uint16_t size, uint32_t timeout) {
    // select handler
    UART_HandleTypeDef *handler;
    if (uart == USART1) {
        handler = &uart_1_h_;
    } else if (uart == USART2) {
        handler = &uart_2_h_;
    } else if (uart == USART3) {
        handler = &uart_3_h_;
    } else if (uart == UART4) {
        handler = &uart_4_h_;
    } else if (uart == UART5) {
        handler = &uart_5_h_;
    } else if (uart == USART6) {
        handler = &uart_6_h_;
    } else {
        return MM_ERROR;
    }
    int8_t result = HAL_UART_Receive(handler, buffer, size, 0);
    return (status_t) result;
}
