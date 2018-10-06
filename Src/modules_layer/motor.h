/*
 * motor.h
 *
 *  Created on: Nov 6, 2016
 *      Author: Bumsik Kim
 */

#ifndef MOTOR_H_
#define MOTOR_H_


#include "common.h"
#include "tim.h"

typedef enum {
    CH_RIGHT = 0x0,
    CH_LEFT = 0x1,
    CH_BOTH = 0x4
} motor_ch_t;


#define motorLEFT_PORT          PORTB
#define motorLEFT_PIN           PIN_6
#define motorRIGHT_PORT         PORTB
#define motorRIGHT_PIN          PIN_8
#define LEFT_PWM_TIM            MM_TIM4
#define LEFT_PWM_PORT           PORTB
#define LEFT_PWM_PIN            PIN_7
#define RIGHT_PWM_TIM           MM_TIM4
#define RIGHT_PWM_PORT          PORTB
#define RIGHT_PWM_PIN           PIN_9
#define LEFT_PWM_CHANNEL        CH_4
#define RIGHT_PWM_CHANNEL       CH_4
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
