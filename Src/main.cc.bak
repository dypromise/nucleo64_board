#include "common.h"
#include "adc.h"
#include "system_init.h"

static void on_b1_pressed(void *ctx);

int main() {

    /* Initialize clock and system configuration */
    system_init();

    /* Initialize all configured peripherals */
    peripheral_init();

    /* Set terminal print by default USART2 in Nucleo-64 */
    terminal_init();

    /* Set ADC, this is A5 Pin in Nucleo-64 */
    adc_init_t adc_setting{};
    adc_setting.device = MM_ADC1;
    adc_setting.channel = MM_ADC_CH5;
    adc_setting.adc_port = PORTA;
    adc_setting.adc_pin = PIN_5;
    adc_t adc_handler{};
    adc_init(&adc_handler, &adc_setting);


    /* Set interrupt button */
    gpio_init_t button_pin_setting{};
    button_pin_setting.Mode = GPIO_MODE_INPUT;
    button_pin_setting.Pull = NOPULL;
    gpio_irt_enable(B1_PORT, B1_PIN, &button_pin_setting, FALLING_EDGE);
    gpio_irt_register(B1_PORT, B1_PIN, on_b1_pressed, &adc_handler);

    /* Set LED. This is A0 pin in Nucleo-64 */
    gpio_init_t led_pin_setting{};
    led_pin_setting.Mode = GPIO_MODE_OUTPUT_PP;
    led_pin_setting.Pull = GPIO_NOPULL;
    led_pin_setting.Speed = GPIO_SPEED_LOW;
    gpio_init(LED1_PORT, LED1_PIN, &led_pin_setting);

    /* Endless loop */
    while (true) {
    }

}


void on_b1_pressed(void *ctx) {

    gpio_set(LED1_PORT, LED1_PIN, GPIO_PIN_SET);
    HAL_Delay(500);
    gpio_set(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);
    HAL_Delay(500);
    gpio_set(LED1_PORT, LED1_PIN, GPIO_PIN_SET);
    HAL_Delay(500);
    gpio_set(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);

    auto adc = (adc_t *) ctx;
    uint32_t voltage = 0;
    if (adc_measure(adc, &voltage) != MM_OK) {
        MM_DEBUG_ERROR("ADC WRONG.\n");
    }
    terminal_printf("voltage: %d\n", voltage);
}
