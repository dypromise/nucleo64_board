#include "common.h"
#include "rotary_encoder.h"

// C standard library
#include <stdlib.h>

// base name change. Used with msg(). See @base.h
#ifdef MM_MSG_BASE
#undef MM_MSG_BASE
#define MM_MSG_BASE "ENCODER"
#endif

static TIM_TypeDef *_get_device(rotary_encoder_device_t dev);

static inline TIM_HandleTypeDef *_get_handler(rotary_encoder_t *obj);

static void _enable_device_clk(rotary_encoder_device_t dev);

/******************************************************************************
 * Public API Functions
 ******************************************************************************/
int rotary_encoder_init(rotary_encoder_t *obj, rotary_encoder_init_t *setting) {

    /* Allocate device handler for HAL driver */
    obj->ext_handler = (extlib_handle_t) malloc(sizeof(TIM_HandleTypeDef));
    TIM_HandleTypeDef *handler = _get_handler(obj);
    if (NULL == handler) {
        MM_DEBUG_ERROR("Encoder handler allocation error!\r\n");
        return MM_ERROR;
    }

    /* Associate the TIM device */
    handler->Instance = _get_device(setting->device);

    /* De-initialize before initialization */
    if (HAL_TIM_Encoder_DeInit(handler) != HAL_OK) {
        /* ADC de-initialization Error */
        MM_DEBUG_ERROR("Encoder device de-initialization error!\r\n");
        return MM_ERROR;
    }

    /* Initialize TIM hardware */
    _enable_device_clk(setting->device); // Enable device Clock first

    // TIM device setting
    TIM_Encoder_InitTypeDef rotary_encoder_config;
    handler->Init.Period = 0xffffffff; //0xFFFF
    handler->Init.Prescaler = setting->prescaler;
    handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handler->Init.RepetitionCounter = 0;
    rotary_encoder_config.EncoderMode = TIM_ENCODERMODE_TI12;

    switch (setting->direction) {
        case CW:
            handler->Init.CounterMode = TIM_COUNTERMODE_UP;

            rotary_encoder_config.IC1Polarity = TIM_ICPOLARITY_RISING;
            rotary_encoder_config.IC1Selection = TIM_ICSELECTION_DIRECTTI;
            rotary_encoder_config.IC1Prescaler = TIM_ICPSC_DIV1;
            rotary_encoder_config.IC1Filter = 0xf;

            rotary_encoder_config.IC2Polarity = TIM_ICPOLARITY_RISING;
            rotary_encoder_config.IC2Selection = TIM_ICSELECTION_DIRECTTI;
            rotary_encoder_config.IC2Prescaler = TIM_ICPSC_DIV1;
            rotary_encoder_config.IC2Filter = 0xf;
            break;
        case ANTI_CW:
            handler->Init.CounterMode = TIM_COUNTERMODE_UP;

            rotary_encoder_config.IC1Polarity = TIM_ICPOLARITY_FALLING;
            rotary_encoder_config.IC1Selection = TIM_ICSELECTION_DIRECTTI;
            rotary_encoder_config.IC1Prescaler = TIM_ICPSC_DIV1;
            rotary_encoder_config.IC1Filter = 0xf;

            rotary_encoder_config.IC2Polarity = TIM_ICPOLARITY_RISING;
            rotary_encoder_config.IC2Selection = TIM_ICSELECTION_DIRECTTI;
            rotary_encoder_config.IC2Prescaler = TIM_ICPSC_DIV1;
            rotary_encoder_config.IC2Filter = 0xf;
            break;
        default:
            MM_DEBUG_ERROR("Incorrect encoder direction");
            return MM_ERROR;
    }
    if (MM_OK != HAL_TIM_Encoder_Init(handler, &rotary_encoder_config)) {
        MM_DEBUG_WARNING("Error init encoder!\r\n");
        return MM_ERROR;
    }

    if (tim_port_pin(setting->device, setting->encoder_port1,
                     setting->encoder_pin1, PULLUP) == MM_OK &&
        tim_port_pin(setting->device, setting->encoder_port2,
                     setting->encoder_pin2, PULLUP) == MM_OK)
        return MM_OK;
    else {
        MM_DEBUG_WARNING("Error init encoder pin!\r\n");
        return MM_ERROR;
    }
}


int rotary_encoder_start(rotary_encoder_t *obj) {
    TIM_HandleTypeDef *handler = _get_handler(obj);
    if (NULL == handler) {
        return MM_ERROR;
    }
    int8_t status = HAL_TIM_Encoder_Start(handler, TIM_CHANNEL_ALL);
    if (MM_OK != status) {
        MM_DEBUG_WARNING("Error starting Encoder!\r\n");
    }
    return status;
}


int rotary_encoder_stop(rotary_encoder_t *obj) {
    TIM_HandleTypeDef *handler = _get_handler(obj);
    if (NULL == handler) {
        return MM_ERROR;
    }
    int8_t status = HAL_TIM_Encoder_Stop(handler, TIM_CHANNEL_ALL);
    if (MM_OK != status) {
        MM_DEBUG_WARNING("Error stopping Encoder!\r\n");
    }
    return status;
}


uint32_t rotary_encoder_set(rotary_encoder_t *obj, uint32_t input) {
    TIM_HandleTypeDef *handler = _get_handler(obj);
    if (NULL == handler) {
        return 0;
    }
    handler->Instance->CNT = input;
    __HAL_TIM_SET_COUNTER(handler, input);
    return __HAL_TIM_GET_COUNTER(handler);
}


uint32_t rotary_encoder_count(rotary_encoder_t *obj) {
    TIM_HandleTypeDef *handler = _get_handler(obj);
    if (NULL == handler) {
        return 0;
    }
    return __HAL_TIM_GET_COUNTER(handler);
}

/******************************************************************************
 * Private Functions
 ******************************************************************************/
static TIM_TypeDef *_get_device(rotary_encoder_device_t dev) {
    static TIM_TypeDef *const _device_table[] = {
            TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11,
            TIM12, TIM13, TIM14
    };
    return _device_table[(int) dev];
}


static inline TIM_HandleTypeDef *_get_handler(rotary_encoder_t *obj) {
    return (TIM_HandleTypeDef *) obj->ext_handler;
}


static void _enable_device_clk(rotary_encoder_device_t dev) {
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
        default:
            MM_DEBUG_ERROR("Wrong Timer device!\r\n");
    }
}
