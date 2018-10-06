/*
 * tim.h
 *
 *  Created on: Oct 27, 2016
 *      Author: Bumsik Kim
 */

#ifndef PERIPHERAL_MM_ENCODER_H_
#define PERIPHERAL_MM_ENCODER_H_

/* Includes */
//#include "common.h"
#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif
#if defined(STM32)
/*******************************************************************************
 * Device Specific Enumerations
 ******************************************************************************/
typedef tim_device_t rotary_encoder_device_t;

/*******************************************************************************
 * Common Enumerations
 ******************************************************************************/
typedef struct _rotary_encoder_t {
    extlib_handle_t ext_handler;
} rotary_encoder_t;

typedef enum {
    CW,
    ANTI_CW
} rotary_encoder_dir_t;

/*******************************************************************************
 * API object structures
 ******************************************************************************/
typedef struct {
    rotary_encoder_device_t device;
    rotary_encoder_dir_t direction;
    uint16_t prescaler;
    gpio_port_t encoder_port1;
    gpio_pin_t encoder_pin1;
    gpio_port_t encoder_port2;
    gpio_pin_t encoder_pin2;
} rotary_encoder_init_t;

#else
#error "Please define device driver! " __FILE__ "(e.g. USE_HAL_DRIVER)\n"
#endif

/*******************************************************************************
 * API functions
 ******************************************************************************/
int rotary_encoder_init(rotary_encoder_t *obj, rotary_encoder_init_t *setting);

int rotary_encoder_start(rotary_encoder_t *obj);

int rotary_encoder_stop(rotary_encoder_t *obj);

uint32_t rotary_encoder_set(rotary_encoder_t *obj, uint32_t input);

uint32_t rotary_encoder_count(rotary_encoder_t *obj);

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_MM_ENCODER_H_ */
