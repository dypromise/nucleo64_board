#include "tick.h"
#include "infrared.h"

// ADC objects
static adc_t _adc_left;
static adc_t _adc_right;
static adc_t _adc_front_left;
static adc_t _adc_front_right;

void range_init(void) {
    /* ADC */
    // Left
    adc_init_t adc_setting;
    adc_setting.device = RECEIVER_ADC;
    adc_setting.channel = RECEIVER_L_CHANNEL;
    adc_setting.adc_port = RECEIVER_L_PORT;
    adc_setting.adc_pin = RECEIVER_L_PIN;
    adc_init(&_adc_left, &adc_setting);

    // Right
    adc_setting.device = RECEIVER_ADC;
    adc_setting.channel = RECEIVER_R_CHANNEL;
    adc_setting.adc_port = RECEIVER_R_PORT;
    adc_setting.adc_pin = RECEIVER_R_PIN;
    adc_init(&_adc_right, &adc_setting);

    // Front (actually FR)
    adc_setting.device = RECEIVER_ADC;
    adc_setting.channel = RECEIVER_FR_CHANNEL;
    adc_setting.adc_port = RECEIVER_FR_PORT;
    adc_setting.adc_pin = RECEIVER_FR_PIN;
    adc_init(&_adc_front_right, &adc_setting);

    // Front (actually FL)
    adc_setting.device = RECEIVER_ADC;
    adc_setting.channel = RECEIVER_FL_CHANNEL;
    adc_setting.adc_port = RECEIVER_FL_PORT;
    adc_setting.adc_pin = RECEIVER_FL_PIN;
    adc_init(&_adc_front_left, &adc_setting);

    /* Emitter */
    gpio_init_t GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init(EMITTER_L_PORT, EMITTER_L_PIN, &GPIO_InitStruct);
    gpio_init(EMITTER_R_PORT, EMITTER_R_PIN, &GPIO_InitStruct);
    gpio_init(EMITTER_FL_PORT, EMITTER_FL_PIN, &GPIO_InitStruct);
    gpio_init(EMITTER_FR_PORT, EMITTER_FR_PIN, &GPIO_InitStruct);

    gpio_set(EMITTER_L_PORT, EMITTER_L_PIN, GPIO_PIN_RESET);
    gpio_set(EMITTER_R_PORT, EMITTER_R_PIN, GPIO_PIN_RESET);
    gpio_set(EMITTER_FL_PORT, EMITTER_FL_PIN, GPIO_PIN_RESET);
    gpio_set(EMITTER_FR_PORT, EMITTER_FR_PIN, GPIO_PIN_RESET);
}

void range_get(struct range_data *out, enum range_ch ch) {
    if (NULL == out) {
        return;
    }
    switch (ch) {
        case RANGE_CH_LEFT:
            gpio_set(EMITTER_L_PORT, EMITTER_L_PIN, GPIO_PIN_SET);
            delay_us(60);
            adc_measure(&_adc_left, &(out->left));
            gpio_set(EMITTER_L_PORT, EMITTER_L_PIN, GPIO_PIN_RESET);
            break;
        case RANGE_CH_RIGHT:
            gpio_set(EMITTER_R_PORT, EMITTER_R_PIN, GPIO_PIN_SET);
            delay_us(60);
            adc_measure(&_adc_right, &(out->right));
            gpio_set(EMITTER_R_PORT, EMITTER_R_PIN, GPIO_PIN_RESET);
            break;
        case RANGE_CH_FRONT:
            gpio_set(EMITTER_FL_PORT, EMITTER_FL_PIN, GPIO_PIN_SET);
            delay_us(60);
            adc_measure(&_adc_front_left, &(out->front_left));
            gpio_set(EMITTER_FL_PORT, EMITTER_FL_PIN, GPIO_PIN_RESET);
            gpio_set(EMITTER_FR_PORT, EMITTER_FR_PIN, GPIO_PIN_SET);
            delay_us(60);
            adc_measure(&_adc_front_right, &(out->front_right));
            gpio_set(EMITTER_FR_PORT, EMITTER_FR_PIN, GPIO_PIN_RESET);
            break;
        case RANGE_CH_ALL:
            range_get(out, RANGE_CH_LEFT);
            range_get(out, RANGE_CH_RIGHT);
            range_get(out, RANGE_CH_FRONT);
            break;
        default:
            MM_DEBUG_ERROR("Wrong range channel!\r\n");
    }
}
