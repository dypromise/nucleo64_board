/*
 * spi.h
 *
 *  Created on: Oct 25, 2016
 *      Author: Bumsik Kim
 */

#ifndef PERIPHERAL_MM_ADC_H_
#define PERIPHERAL_MM_ADC_H_

//#include "common.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Forward declaration and matching manufacturer drivers.
 * API users can ignore this section
 ******************************************************************************/
#if defined(STM32)
typedef ADC_HandleTypeDef adc_handle_t;
#else
#error "Please include config.h in the source file or define device driver! "
"(e.g. USE_HAL_DRIVER)."
#endif

/*******************************************************************************
 * Device Specific Enumerations
 ******************************************************************************/
#if defined(STM32)
typedef enum _adc_device_t {
    MM_ADC1 = 1,
    MM_ADC2,
    MM_ADC3
} adc_device_t;

typedef enum _adc_ch_t {
    MM_ADC_CH0 = 0,
    MM_ADC_CH1,
    MM_ADC_CH2,
    MM_ADC_CH3,
    MM_ADC_CH4,
    MM_ADC_CH5,
    MM_ADC_CH6,
    MM_ADC_CH7,
    MM_ADC_CH8,
    MM_ADC_CH9,
    MM_ADC_CH10,
    MM_ADC_CH11,
    MM_ADC_CH12,
    MM_ADC_CH13,
    MM_ADC_CH14,
    MM_ADC_CH15
} adc_ch_t;
#else
#error "Please define device driver! " __FILE__ "(e.g. USE_HAL_DRIVER)\n"
#endif

/*******************************************************************************
 * API object structures
 ******************************************************************************/
typedef struct _adc_t {
    adc_handle_t handler;
#ifdef MM_USE_PERIPHERAL_FREERTOS_LOCK
    SemaphoreHandle_t lock; // mutex lock
#endif
} adc_t;

typedef struct _adc_init_t {
    adc_device_t device;
    adc_ch_t channel;
} adc_init_t;

/*******************************************************************************
 * API functions
 ******************************************************************************/
/* Initialization functions */
int adc_init(adc_t *obj, adc_init_t *settings);

int adc_pin_set(gpio_port_t port, gpio_pin_t pin);

/* Operation functions */
uint32_t adc_measure(adc_t *adc);


#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_MM_ADC_H_ */
