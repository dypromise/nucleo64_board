/*
 * motor.h
 *
 *  Created on: Nov 6, 2016
 *      Author: Bumsik Kim
 */

#ifndef MOTOR_H_
#define MOTOR_H_


#include "common.h"

typedef enum {
    CH_RIGHT = 0x0,
    CH_LEFT = 0x1,
    CH_BOTH = 0x4
} motor_ch_t;


#define motorLEFT_PORT GPIOB
#define motorLEFT_PIN GPIO_PIN_6
#define motorRIGHT_PORT GPIOB
#define motorRIGHT_PIN GPIO_PIN_8
#define LEFT_PWM_TIM (tim_device_t)4
#define LEFT_PWM_PORT GPIOB
#define LEFT_PWM_PIN GPIO_PIN_7
#define RIGHT_PWM_TIM (tim_device_t)4
#define RIGHT_PWM_PORT GPIOB
#define RIGHT_PWM_PIN GPIO_PIN_9
#define LEFT_PWM_CHANNEL (tim_ch_t)2
#define RIGHT_PWM_CHANNEL (tim_ch_t)4
#ifdef __cplusplus
extern "C" {
#endif

status_t motor_init(void);

void motor_dir_forward(void);

void motor_dir_backward(void);

void motor_dir_rotate_left(void);

void motor_dir_rotate_right(void);

int32_t motor_speed_percent(motor_ch_t channel, int32_t speed);

int32_t motor_speed_permyriad(motor_ch_t channel, int32_t speed);

status_t motor_start(motor_ch_t eChannel);

status_t motor_stop(motor_ch_t eChannel);

#ifdef __cplusplus
}
#endif


#endif /* MOTOR_H_ */
