#include "motor.h"
#include "tim.h"
#include "gpio.h"


static tim_t _timer;

static inline void left_set_toggle_(void) {
    gpio_toggle(motorLEFT_PORT, motorLEFT_PIN);
}

static inline void right_set_toggle_(void) {
    gpio_toggle(motorRIGHT_PORT, motorRIGHT_PIN);
}

static inline void left_set_forward_(void) {
    gpio_set(motorLEFT_PORT, motorLEFT_PIN, GPIO_PIN_SET);
}

static inline void right_set_forward_(void) {
    gpio_set(motorRIGHT_PORT, motorRIGHT_PIN, GPIO_PIN_SET);
}

static inline void left_set_backward_() {
    gpio_set(motorLEFT_PORT, motorLEFT_PIN, GPIO_PIN_RESET);
}

static inline void right_set_backward_(void) {
    gpio_set(motorRIGHT_PORT, motorRIGHT_PIN, GPIO_PIN_RESET);
}


/**
 * Function Implementations
 */
status_t motor_init(void) {

    /* init GPIO */
    gpio_init_t gpio_setting = {
            .Pull = NOPULL,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Speed = GPIO_SPEED_HIGH
    };
    gpio_init(motorLEFT_PORT, motorLEFT_PIN, &gpio_setting);
    gpio_setting.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init(motorRIGHT_PORT, motorRIGHT_PIN, &gpio_setting);

    tim_port_pin(LEFT_PWM_TIM, LEFT_PWM_PORT, LEFT_PWM_PIN,
                 NOPULL);    //left  PWM
    tim_port_pin(RIGHT_PWM_TIM, RIGHT_PWM_PORT, RIGHT_PWM_PIN,
                 NOPULL);    //right PWM

    motor_dir_forward();

    // PWM settings
    pwm_init_t pwm_setting = {    // PWM is at 10kHz
            .device = LEFT_PWM_TIM,
            .clock_frequency = 180000000,
            .period = 10000
    };
    pwm_init(&_timer, &pwm_setting);

    /* Firstly set speed 0 */
    motor_speed_percent(CH_BOTH, 0);
    return MM_OK;
}


int32_t motor_speed_percent(motor_ch_t channel, int32_t speed) {
    int go_backword = 0;
    if (speed < 0) {
        go_backword = 1;
        speed *= -1;
    }
    switch (channel) {
        case CH_LEFT:
            /* Set the pulse value for channel 1 */
            if (go_backword) {
                left_set_backward_();
            } else {
                left_set_forward_();
            }
            pwm_percent(&_timer, LEFT_PWM_CHANNEL, speed);
            break;

        case CH_RIGHT:
            /* Set the pulse value for channel 4 */
            if (go_backword) {
                right_set_backward_();
            } else {
                right_set_forward_();
            }
            pwm_percent(&_timer, RIGHT_PWM_CHANNEL, speed);
            break;

        case CH_BOTH:
            /* Set the pulse value for all channel */
            if (go_backword) {
                left_set_backward_();
                right_set_backward_();
            } else {
                left_set_forward_();
                right_set_forward_();
            }
            pwm_percent(&_timer, LEFT_PWM_CHANNEL, speed);
            pwm_percent(&_timer, RIGHT_PWM_CHANNEL, speed);
            break;

        default:
            /* Return Error in other cases */
            return 0;
    }

    /* return OK */
    return speed;
}

int32_t motor_speed_permyriad(motor_ch_t channel, int32_t speed) {
    int go_backword = 0;
    if (speed < 0) {
        go_backword = 1;
        speed *= -1;
    }
    switch (channel) {
        case CH_LEFT:
            /* Set the pulse value for channel 1 */
            if (go_backword) {
                left_set_backward_();
            } else {
                left_set_forward_();
            }
            pwm_permyriad(&_timer, LEFT_PWM_CHANNEL, speed);
            break;

        case CH_RIGHT:
            /* Set the pulse value for channel 4 */
            if (go_backword) {
                right_set_backward_();
            } else {
                right_set_forward_();
            }
            pwm_permyriad(&_timer, RIGHT_PWM_CHANNEL, speed);
            break;

        case CH_BOTH:
            /* Set the pulse value for all channel */
            if (go_backword) {
                left_set_backward_();
                right_set_backward_();
            } else {
                left_set_forward_();
                right_set_forward_();
            }
            pwm_permyriad(&_timer, LEFT_PWM_CHANNEL, speed);
            pwm_permyriad(&_timer, RIGHT_PWM_CHANNEL, speed);
            break;

        default:
            /* Return Error in other cases */
            return 0;
    }

    /* return OK */
    return speed;
}

void motor_dir_forward(void) {
    right_set_forward_();
    left_set_forward_();
}

void motor_dir_backward(void) {
    right_set_backward_();
    left_set_backward_();
}

void motor_dir_rotate_right(void) {
    right_set_backward_();
    left_set_forward_();
}

void motor_dir_rotate_left(void) {
    right_set_forward_();
    left_set_backward_();
}

status_t motor_start(motor_ch_t channel) {
    status_t result;
    switch (channel) {
        case CH_LEFT:
            /* Start channel 1 */
            result = pwm_start(&_timer, LEFT_PWM_CHANNEL);
            if (result != MM_OK) {
                return result;
            }
            break;
        case CH_RIGHT:
            /* Start channel 4 */
            result = pwm_start(&_timer, RIGHT_PWM_CHANNEL);
            if (result != MM_OK) {
                return result;
            }
            break;
        case CH_BOTH:
            /* Start channel 1 */
            result = pwm_start(&_timer, LEFT_PWM_CHANNEL);
            if (result != MM_OK) {
                return result;
            }
            /* Start channel 4 */
            result = pwm_start(&_timer, RIGHT_PWM_CHANNEL);
            if (result != MM_OK) {
                return result;
            }
            break;
        default:
            return MM_ERROR;
    }
    /* Return OK */
    return MM_OK;
}

status_t motor_stop(motor_ch_t channel) {
    status_t result;
    switch (channel) {
        case CH_LEFT:
            /* Start channel 1 */
            result = pwm_stop(&_timer, LEFT_PWM_CHANNEL);
            if (result != MM_OK) {
                return result;
            }
            break;
        case CH_RIGHT:
            /* Start channel 4 */
            result = pwm_stop(&_timer, RIGHT_PWM_CHANNEL);
            if (result != MM_OK) {
                return result;
            }
            break;
        case CH_BOTH:
            /* Start channel 1 */
            result = pwm_stop(&_timer, LEFT_PWM_CHANNEL);
            if (result != MM_OK) {
                return result;
            }
            /* Start channel 4 */
            result = pwm_stop(&_timer, RIGHT_PWM_CHANNEL);
            if (result != MM_OK) {
                return result;
            }
            break;

        default:
            return MM_ERROR;
    }
    /* Return OK */
    return MM_OK;
}
