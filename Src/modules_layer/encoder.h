/*
 * encoder.h
 *
 *  Created on: Nov 6, 2016
 *      Author: Bumsik Kim
 */

#ifndef ENCODER_H_
#define ENCODER_H_


#include <stdint-gcc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENCODER_L_TIMER         2
#define ENCODER_L_CHA_PORT      GPIOA
#define ENCODER_L_CHA_PIN       GPIO_PIN_15
#define ENCODER_L_PIN_PULLUP    GPIO_PULLUP
#define ENCODER_L_CHB_PORT      GPIOB
#define ENCODER_L_CHB_PIN       GPIO_PIN_3
#define ENCODER_R_TIMER         5
#define ENCODER_R_CHA_PORT      GPIOA
#define ENCODER_R_CHA_PIN       GPIO_PIN_0
#define ENCODER_R_PIN_PULLUP    GPIO_PULLUP
#define ENCODER_R_CHB_PORT      GPIOA
#define ENCODER_R_CHB_PIN       GPIO_PIN_1

struct encoder_data {
    int32_t left;
    int32_t right;
};

enum encoder_ch {
    ENCODER_CH_LEFT,
    ENCODER_CH_RIGHT,
    ENCODER_CH_BOTH
};

void encoder_init(void);

void encoder_get(struct encoder_data *out, enum encoder_ch ch);

void encoder_reset(enum encoder_ch ch);

#ifdef __cplusplus
}
#endif


#endif /* ENCODER_H_ */
