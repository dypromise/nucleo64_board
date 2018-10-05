/*
 * tim.h
 *
 *  Created on: Oct 27, 2016
 *      Author: Bumsik Kim
 */

#ifndef PERIPHERAL_MM_TIMER_H_
#define PERIPHERAL_MM_TIMER_H_

/* Includes */
#include <stm32f4xx_hal.h>
#include "common.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Device Specific Enumerations
 ******************************************************************************/
#if defined(STM32)
typedef enum _tim_device_t {
    MM_TIM1 = 0,
    MM_TIM2,
    MM_TIM3,
    MM_TIM4,
    MM_TIM5,
    MM_TIM6,
    MM_TIM7,
    MM_TIM8,
    MM_TIM9,
    MM_TIM10,
    MM_TIM11,
    MM_TIM12,
    MM_TIM13,
    MM_TIM14
} tim_device_t;

typedef enum _tim_ch_t {
    CH_1 = TIM_CHANNEL_1, // 0x01U,
    CH_2 = TIM_CHANNEL_2, // 0x02U,
    CH_3 = TIM_CHANNEL_3, // 0x03U,
    CH_4 = TIM_CHANNEL_4  //0x04U,
} tim_ch_t;
#else
#error "Please define device driver! " __FILE__ "(e.g. USE_HAL_DRIVER)\n"
#endif


/*******************************************************************************
 * Common Enumerations
 ******************************************************************************/
typedef struct _tim_t {
    extlib_handle_t ext_handler;
} tim_t;

/*******************************************************************************
 * API object structures
 ******************************************************************************/
typedef struct {
    tim_device_t device;
    uint32_t clock_frequency;
    uint16_t period;
} pwm_init_t;


/*******************************************************************************
 * API functions
 ******************************************************************************/
// TODO: generic timer function
int tim_port_pin(tim_device_t dev, gpio_port_t port, gpio_pin_t pin,
                 gpio_pull_t pull);

// PWM functions
int pwm_init(tim_t *obj, pwm_init_t *setting);

int pwm_percent(tim_t *obj, tim_ch_t ch, uint8_t duty_cycle_percent);

// duty_cycle_permyriad range from 0 to 100.
int pwm_permyriad(tim_t *obj, tim_ch_t ch, uint16_t duty_cycle_permyriad);

status_t pwm_start(tim_t *obj, tim_ch_t ch);

status_t pwm_stop(tim_t *obj, tim_ch_t ch);

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_MM_TIMER_H_ */
