#include "common.h"
#include "gpio.h"

// base name change. Used with msg(). See @base.h
#ifdef MM_MSG_BASE
#undef MM_MSG_BASE
#define MM_MSG_BASE "GPIO"
#endif

static int
register_callback_(gpio_pin_t pin, void (*callback)(void *), void *args);

static int set_isr_(gpio_pin_t pin, uint8_t enable);

static void (*callback_func_list_[16])(void *);

static void (*callback_arg_list_[16])(void *);

/******************************************************************************
 * GPIO APIs
 ******************************************************************************/
void gpio_init(gpio_port_t port, gpio_pin_t pin, gpio_init_t *gpio_init) {
    gpio_enable_clk(port);
    gpio_init->Pin = pin;
    HAL_GPIO_Init(port, gpio_init);
}

gpio_pin_state gpio_read(gpio_port_t port, gpio_pin_t pin) {
    return HAL_GPIO_ReadPin(port, pin);
}

void gpio_set(gpio_port_t port, gpio_pin_t pin, gpio_pin_state state) {
    HAL_GPIO_WritePin(port, pin, state);
}


void gpio_toggle(gpio_port_t port, gpio_pin_t pin) {
    HAL_GPIO_TogglePin(port, pin);
}

/* GPIO Ports Clock Enable */
void gpio_enable_clk(gpio_port_t port) {
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    } else if (port == GPIOH) {
        __HAL_RCC_GPIOH_CLK_ENABLE();
    } else {
        MM_DEBUG_ERROR("Wrong port selected for clock!\r\n");
    }
}

/******************************************************************************
 * GPIO external interrupt APIs
 ******************************************************************************/
int
gpio_irt_register(gpio_port_t port, gpio_pin_t pin, void (*callback)(void *),
                  void *args) {
    return register_callback_(pin, callback, args);
}


int gpio_irt_deregister(gpio_port_t port, gpio_pin_t pin) {
    return register_callback_(pin, NULL, NULL);
}


int gpio_irt_enable(gpio_port_t port, gpio_pin_t pin, gpio_init_t *gpio_setting,
                    gpio_edge_t edge) {
    gpio_setting->Mode = ((edge == RISING_EDGE) ? GPIO_MODE_IT_RISING :
                          (edge == FALLING_EDGE) ? GPIO_MODE_IT_FALLING :
                          (edge == BOTH_EDGE) ? GPIO_MODE_IT_RISING_FALLING
                                              : 0);
    /* Include gpio_init function */
    gpio_init(port, pin, gpio_setting);

    /* Enable interrupt */
    return set_isr_(pin, 1);
}


int gpio_irt_disable(gpio_port_t port, gpio_pin_t pin) {
    gpio_init_t gpio_setting;
    gpio_setting.Mode = GPIO_MODE_INPUT;
    gpio_setting.Pull = NOPULL;
    gpio_setting.Speed = GPIO_SPEED_LOW;
    gpio_init(port, pin, &gpio_setting);
    return set_isr_(pin, 0);
}

/******************************************************************************
 * Privates
 ******************************************************************************/
static int
register_callback_(gpio_pin_t pin, void (*callback)(void *), void *args) {
    // Set NVIC and put the callback
    int idx;
    switch (pin) {
        case PIN_0:
            idx = 0;
            break;
        case PIN_1:
            idx = 1;
            break;
        case PIN_2:
            idx = 2;
            break;
        case PIN_3:
            idx = 3;
            break;
        case PIN_4:
            idx = 4;
            break;
        case PIN_5:
            idx = 5;
            break;
        case PIN_6:
            idx = 6;
            break;
        case PIN_7:
            idx = 7;
            break;
        case PIN_8:
            idx = 8;
            break;
        case PIN_9:
            idx = 9;
            break;
        case PIN_10:
            idx = 10;
            break;
        case PIN_11:
            idx = 11;
            break;
        case PIN_12:
            idx = 12;
            break;
        case PIN_13:
            idx = 13;
            break;
        case PIN_14:
            idx = 14;
            break;
        case PIN_15:
            idx = 15;
            break;
        default:
            MM_DEBUG_ERROR("Wrong Pin selected!\r\n");
            return MM_ERROR;
    }
    if (NULL != callback_func_list_[idx]) {
        MM_DEBUG_WARNING("Replacing an interrupt handler...\r\n");
    }
    callback_func_list_[idx] = callback;
    callback_arg_list_[idx] = args;
    return MM_OK;
}

static int set_isr_(gpio_pin_t pin, uint8_t enable) {
    // Set NVIC and put the callback
    IRQn_Type irq_num;
    switch (pin) {
        case PIN_0:
            irq_num = EXTI0_IRQn;
            break;
        case PIN_1:
            irq_num = EXTI1_IRQn;
            break;
        case PIN_2:
            irq_num = EXTI2_IRQn;
            break;
        case PIN_3:
            irq_num = EXTI3_IRQn;
            break;
        case PIN_4:
            irq_num = EXTI4_IRQn;
            break;
        case PIN_5:
            irq_num = EXTI9_5_IRQn;
            break;
        case PIN_6:
            irq_num = EXTI9_5_IRQn;
            break;
        case PIN_7:
            irq_num = EXTI9_5_IRQn;
            break;
        case PIN_8:
            irq_num = EXTI9_5_IRQn;
            break;
        case PIN_9:
            irq_num = EXTI9_5_IRQn;
            break;
        case PIN_10:
            irq_num = EXTI15_10_IRQn;
            break;
        case PIN_11:
            irq_num = EXTI15_10_IRQn;
            break;
        case PIN_12:
            irq_num = EXTI15_10_IRQn;
            break;
        case PIN_13:
            irq_num = EXTI15_10_IRQn;
            break;
        case PIN_14:
            irq_num = EXTI15_10_IRQn;
            break;
        case PIN_15:
            irq_num = EXTI15_10_IRQn;
            break;
        default:
            MM_DEBUG_ERROR("Wrong Pin selected!\r\n");
            return MM_ERROR;
    }
    HAL_NVIC_SetPriority(irq_num, 15, 0);  // TODO: Find a better group number

    // enable interrupt
    if (enable) {
        HAL_NVIC_EnableIRQ(irq_num);
    } else {
        HAL_NVIC_DisableIRQ(irq_num);
    }
    return MM_OK;
}

/******************************************************************************
 * Interrupt Handlers
 ******************************************************************************/
void EXTI0_IRQHandler(void) {
    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
        callback_func_list_[0](callback_arg_list_[0]);

    }
}

void EXTI1_IRQHandler(void) {
    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
        callback_func_list_[1](callback_arg_list_[1]);
    }
}

void EXTI2_IRQHandler(void) {
    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
        callback_func_list_[2](callback_arg_list_[2]);
    }
}

void EXTI3_IRQHandler(void) {
    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
        callback_func_list_[3](callback_arg_list_[3]);
    }
}

void EXTI4_IRQHandler(void) {
    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
        callback_func_list_[4](callback_arg_list_[4]);
    }
}

void EXTI9_5_IRQHandler(void) {
    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
        callback_func_list_[5](callback_arg_list_[5]);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
        callback_func_list_[6](callback_arg_list_[6]);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
        callback_func_list_[7](callback_arg_list_[7]);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
        callback_func_list_[8](callback_arg_list_[8]);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
        callback_func_list_[9](callback_arg_list_[9]);
    }
}

void EXTI15_10_IRQHandler(void) {
    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
        callback_func_list_[10](callback_arg_list_[10]);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
        callback_func_list_[11](callback_arg_list_[11]);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
        callback_func_list_[12](callback_arg_list_[12]);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
        callback_func_list_[13](callback_arg_list_[13]);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
        callback_func_list_[14](callback_arg_list_[14]);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
        callback_func_list_[15](callback_arg_list_[15]);
    }
}
