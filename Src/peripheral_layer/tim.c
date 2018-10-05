#include <common.h>
#include <stdlib.h>
#include "tim.h"

// base name change. Used with msg(). See @base.h
#ifdef MM_MSG_BASE
#undef MM_MSG_BASE
#define MM_MSG_BASE "TIMER"
#endif

static TIM_TypeDef *_get_device(tim_device_t dev);

static inline TIM_HandleTypeDef *_get_handler(tim_t *obj);

static uint32_t _get_bus_freq(tim_device_t dev);

static void _enable_device_clk(tim_device_t dev);


/******************************************************************************
 * Public API Functions
 ******************************************************************************/
int tim_port_pin(tim_device_t dev, gpio_port_t port, gpio_pin_t pin,
                 gpio_pull_t pull) {

    uint32_t alternate = MM_WRONG_PIN;
    /* Alternative Func Pin */
    switch (dev) {
        case MM_TIM1:
            alternate = GPIO_AF1_TIM1;
            break;
        case MM_TIM2:
            alternate = GPIO_AF1_TIM2;
            break;
        case MM_TIM3:
            alternate = GPIO_AF2_TIM3;
            break;
        case MM_TIM4:
            alternate = GPIO_AF2_TIM4;
            break;
        case MM_TIM5:
            alternate = GPIO_AF2_TIM5;
            break;
        case MM_TIM8:
            alternate = GPIO_AF3_TIM8;
            break;
        case MM_TIM9:
            alternate = GPIO_AF3_TIM9;
            break;
        case MM_TIM10:
            alternate = GPIO_AF3_TIM10;
            break;
        case MM_TIM11:
            alternate = GPIO_AF3_TIM11;
            break;
        default:
            MM_DEBUG_ERROR("Choose correct TIM!\r\n");
            return MM_ERROR;
    }
    if (alternate == MM_WRONG_PIN) {
        MM_DEBUG_ERROR("Wrong MOSI pin! Find a correct one.\r\n");
        return MM_ERROR;
    }
    gpio_enable_clk(port);
    gpio_init_t gpio_setting = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = pull,
            .Alternate = alternate,
            .Speed = GPIO_SPEED_FREQ_LOW
    };
    gpio_init(port, pin, &gpio_setting);
    return MM_OK;
}

// PWM functions
int pwm_init(tim_t *obj, pwm_init_t *setting) {

    /* allocate device handler for HAL driver */
    obj->ext_handler = (extlib_handle_t) malloc(sizeof(TIM_HandleTypeDef));
    TIM_HandleTypeDef *handler = _get_handler(obj);
    if (NULL == handler) {
        MM_DEBUG_ERROR("Memory allocation error!\r\n");
        return MM_ERROR;
    }

    /* Associate the TIM device */
    handler->Instance = _get_device(setting->device);

    /* De-initialize before initialization */
    if (HAL_TIM_PWM_DeInit(handler) != HAL_OK) {
        /* ADC de-initialization Error */
        MM_DEBUG_ERROR("device de-initialization error!\r\n");
        return MM_ERROR;
    }

    /* Initialize TIM hardware */
    _enable_device_clk(setting->device); // Enable device Clock first

    /* Timer init */
    handler->Init.CounterMode = TIM_COUNTERMODE_UP;
    handler->Init.Period = setting->period;
    handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handler->Init.Prescaler =
            _get_bus_freq(setting->device) / setting->clock_frequency - 1;
    uint32_t freq;
    freq = _get_bus_freq(setting->device) / (handler->Init.Prescaler + 1);

    MM_DEBUG_MSG("Requested clock freq: %lu\r\n", setting->clock_frequency);
    MM_DEBUG_MSG("Requested pulse freq: %lu\r\n",
                 setting->clock_frequency / setting->period);
    MM_DEBUG_MSG("Selected clock freq: %lu\r\n", freq);
    MM_DEBUG_MSG("Selected pulse freq: %lu\r\n", freq / handler->Init.Period);

    int8_t status = HAL_TIM_PWM_Init(handler);
    if (MM_OK != status) {
        MM_DEBUG_WARNING("Error init timer device!\r\n");
    }
    return (status_t) status;
}


int pwm_percent(tim_t *obj, tim_ch_t channel, uint8_t duty_cycle_percent) {
    return pwm_permyriad(obj, channel, duty_cycle_percent * 100);
}

int pwm_permyriad(tim_t *obj, tim_ch_t channel, uint16_t duty_cycle_permyriad) {
    // select handler
    TIM_HandleTypeDef *handler = _get_handler(obj);
    if (NULL == handler) {
        return MM_ERROR;
    }
    // make configuration
    TIM_OC_InitTypeDef config;
    config.OCMode = TIM_OCMODE_PWM1;
    config.OCPolarity = TIM_OCPOLARITY_HIGH;
    config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    config.OCFastMode = TIM_OCFAST_DISABLE;
    config.OCIdleState = TIM_OCIDLESTATE_RESET;
    config.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    // setting duty cycle
    if (duty_cycle_permyriad > 10000) {
        MM_DEBUG_WARNING("Duty cycle is > 10,000 permyriad.\r\n");
        duty_cycle_permyriad = 10000;
    }
    uint32_t period = handler->Init.Period;
    uint32_t pulse_width = (period * duty_cycle_permyriad) / 10000;
    config.Pulse = pulse_width;

    int8_t status = HAL_TIM_PWM_ConfigChannel(handler, &config, channel);
    if (MM_OK != status) {
        MM_DEBUG_WARNING("Error setting PWM ch!\r\n");
    }
    return (status_t) status;
}


status_t pwm_start(tim_t *obj, tim_ch_t channel) {
    // get handler and enable timer
    TIM_HandleTypeDef *handler = _get_handler(obj);
    if (NULL == handler) {
        return MM_ERROR;
    }
    status_t status;
    status = HAL_TIM_PWM_Start(handler, (uint32_t) channel);
    if (MM_OK != status) {
        MM_DEBUG_WARNING("Error starting PWM ch!\r\n");
    }
    return status;
}


status_t pwm_stop(tim_t *obj, tim_ch_t channel) {
    // get handler and enable timer
    TIM_HandleTypeDef *handler = _get_handler(obj);
    if (NULL == handler) {
        return MM_ERROR;
    }
    status_t status;
    switch (channel) {
        case CH_1:
            status = HAL_TIM_PWM_Stop(handler, TIM_CHANNEL_1);
            break;
        case CH_2:
            status = HAL_TIM_PWM_Stop(handler, TIM_CHANNEL_2);
            break;
        case CH_3:
            status = HAL_TIM_PWM_Stop(handler, TIM_CHANNEL_3);
            break;
        case CH_4:
            status = HAL_TIM_PWM_Stop(handler, TIM_CHANNEL_4);
            break;
        default:
            MM_DEBUG_ERROR("Choose correct channel!\r\n");
            return MM_ERROR;
    }
    if (MM_OK != status) {
        MM_DEBUG_WARNING("Error stopping PWM ch!\r\n");
    }
    return status;
}


/******************************************************************************
 * Private Functions
 ******************************************************************************/
static TIM_TypeDef *_get_device(tim_device_t dev) {
    static TIM_TypeDef *const _device_table[] = {
            TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11,
            TIM12, TIM13, TIM14
    };
    return _device_table[(int) dev];
}


static inline TIM_HandleTypeDef *_get_handler(tim_t *obj) {
    return (TIM_HandleTypeDef *) obj->ext_handler;
}

static uint32_t _get_bus_freq(tim_device_t dev) {
    RCC_ClkInitTypeDef rcc_config;
    uint32_t flash_latency;
    uint8_t tim_multiplier = 1;
    HAL_RCC_GetClockConfig(&rcc_config, &flash_latency);
    if ((dev == MM_TIM2) || (dev == MM_TIM3) ||
        (dev == MM_TIM4) || (dev == MM_TIM5) ||
        (dev == MM_TIM6) || (dev == MM_TIM7) ||
        (dev == MM_TIM12) || (dev == MM_TIM13) ||
        (dev == MM_TIM14)) {    // APB1
        if (rcc_config.APB1CLKDivider !=
            RCC_HCLK_DIV1) {    // See RCC datasheet
            tim_multiplier = 2;
        }
        return HAL_RCC_GetPCLK1Freq() * tim_multiplier;
    } else if ((dev == MM_TIM1) || (dev == MM_TIM8) ||
               (dev == MM_TIM9) || (dev == MM_TIM10) ||
               (dev == MM_TIM11)) {    // APB2
        if (rcc_config.APB2CLKDivider !=
            RCC_HCLK_DIV1) {    // See RCC datasheet
            tim_multiplier = 2;
        }
        return HAL_RCC_GetPCLK2Freq() * tim_multiplier;
    }
    MM_DEBUG_ERROR("Wrong TIM device! Find a correct one.\r\n");
    return 0;
}


static void _enable_device_clk(tim_device_t dev) {
    switch (dev) {
        case MM_TIM1:
            __HAL_RCC_TIM1_CLK_ENABLE();
            break;
        case MM_TIM2:
            __HAL_RCC_TIM2_CLK_ENABLE();
            break;
        case MM_TIM3:
            __HAL_RCC_TIM3_CLK_ENABLE();
            break;
        case MM_TIM4:
            __HAL_RCC_TIM4_CLK_ENABLE();
            break;
        case MM_TIM5:
            __HAL_RCC_TIM5_CLK_ENABLE();
            break;
        case MM_TIM6:
            __HAL_RCC_TIM6_CLK_ENABLE();
            break;
        case MM_TIM7:
            __HAL_RCC_TIM7_CLK_ENABLE();
            break;
        case MM_TIM8:
            __HAL_RCC_TIM8_CLK_ENABLE();
            break;
        case MM_TIM9:
            __HAL_RCC_TIM9_CLK_ENABLE();
            break;
        case MM_TIM10:
            __HAL_RCC_TIM10_CLK_ENABLE();
            break;
        case MM_TIM11:
            __HAL_RCC_TIM11_CLK_ENABLE();
            break;
        case MM_TIM12:
            __HAL_RCC_TIM12_CLK_ENABLE();
            break;
        case MM_TIM13:
            __HAL_RCC_TIM13_CLK_ENABLE();
            break;
        case MM_TIM14:
            __HAL_RCC_TIM14_CLK_ENABLE();
            break;
        default:
            MM_DEBUG_ERROR("Wrong TIM device!\r\n");
    }
}
