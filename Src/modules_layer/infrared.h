
#ifndef SRC_BOARD_WOLFIEMOUSE_RANGE_H_
#define SRC_BOARD_WOLFIEMOUSE_RANGE_H_


#include "stdlib.h"

#ifdef STM32

#include "adc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RECEIVER_ADC        MM_ADC1
#define RECEIVER_L_CHANNEL  MM_ADC_CH8
#define RECEIVER_L_PORT     GPIOB
#define RECEIVER_L_PIN      GPIO_PIN_0
#define RECEIVER_R_CHANNEL  MM_ADC_CH4
#define RECEIVER_R_PORT     GPIOA
#define RECEIVER_R_PIN      GPIO_PIN_4
#define RECEIVER_FR_CHANNEL MM_ADC_CH15
#define RECEIVER_FR_PORT    GPIOC
#define RECEIVER_FR_PIN     GPIO_PIN_5
#define RECEIVER_FL_CHANNEL MM_ADC_CH7
#define RECEIVER_FL_PORT    GPIOA
#define RECEIVER_FL_PIN     GPIO_PIN_7
#define EMITTER_L_PORT      GPIOH
#define EMITTER_L_PIN       GPIO_PIN_1
#define EMITTER_R_PORT      GPIOC
#define EMITTER_R_PIN       GPIO_PIN_14
#define EMITTER_FL_PORT     GPIOH
#define EMITTER_FL_PIN      GPIO_PIN_0
#define EMITTER_FR_PORT     GPIOC
#define EMITTER_FR_PIN      GPIO_PIN_15
#define RANGE_MAX           4095

#else
#error "Please define device driver! " __FILE__ "\n"
#endif

struct range_data {
    uint32_t left;
    uint32_t right;
    uint32_t front_left;
    uint32_t front_right;
};

enum range_ch {
    RANGE_CH_LEFT,
    RANGE_CH_RIGHT,
    RANGE_CH_FRONT,
    RANGE_CH_ALL
};

void range_init(void);

void range_get(struct range_data *out, enum range_ch ch);

#ifdef __cplusplus
}
#endif


#endif /* ENCODER_H_ */
