#include "encoder.h"
#include "config_measurements.h"
#include "rotary_encoder.h"

static rotary_encoder_t _left;
static rotary_encoder_t _right;

void encoder_init(void) {
    // init left channel
    tim_port_pin((tim_device_t) ENCODER_L_TIMER, ENCODER_L_CHA_PORT,
                 ENCODER_L_CHA_PIN,
                 ENCODER_L_PIN_PULLUP);
    tim_port_pin((tim_device_t) ENCODER_L_TIMER, ENCODER_L_CHB_PORT,
                 ENCODER_L_CHB_PIN,
                 ENCODER_L_PIN_PULLUP);

    // init right channel
    tim_port_pin((tim_device_t) ENCODER_R_TIMER, ENCODER_R_CHA_PORT,
                 ENCODER_R_CHA_PIN,
                 ENCODER_R_PIN_PULLUP);
    tim_port_pin((tim_device_t) ENCODER_R_TIMER, ENCODER_R_CHB_PORT,
                 ENCODER_R_CHB_PIN,
                 ENCODER_R_PIN_PULLUP);

    // init timer
    // left channel
    rotary_encoder_init_t enc_setting = {
            .device = (rotary_encoder_device_t) ENCODER_L_TIMER,
            .direction = ANTI_CW,
            .prescaler = 0x0U
    };
    rotary_encoder_init(&_left, &enc_setting);

    // right channel
    enc_setting = (rotary_encoder_init_t) {
            .device = (rotary_encoder_device_t) ENCODER_R_TIMER,
            .direction = CW,
            .prescaler = 0x0U
    };
    rotary_encoder_init(&_right, &enc_setting);

    // start encoders
    rotary_encoder_start(&_left);
    rotary_encoder_start(&_right);

    encoder_reset(ENCODER_CH_BOTH);
}

void encoder_get(struct encoder_data *out, enum encoder_ch ch) {
    if (NULL == out) {
        return;
    }
    // Get
    switch (ch) {
        case ENCODER_CH_LEFT:
            out->left = rotary_encoder_count(&_left);
            break;
        case ENCODER_CH_RIGHT:
            out->right = rotary_encoder_count(&_right);
            break;
        case ENCODER_CH_BOTH:
            out->left = rotary_encoder_count(&_left);
            out->right = rotary_encoder_count(&_right);
            break;
    }
}

void encoder_reset(enum encoder_ch ch) {
    // Get
    switch (ch) {
        case ENCODER_CH_LEFT:
            rotary_encoder_set(&_left, MEASURE_ENCODER_DEFAULT);
            break;
        case ENCODER_CH_RIGHT:
            rotary_encoder_set(&_right, MEASURE_ENCODER_DEFAULT);
            break;
        case ENCODER_CH_BOTH:
            rotary_encoder_set(&_left, MEASURE_ENCODER_DEFAULT);
            rotary_encoder_set(&_right, MEASURE_ENCODER_DEFAULT);
            break;
    }
}
