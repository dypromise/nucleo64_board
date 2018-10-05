#include <mouse_controller.h>
#include <common.h>
#include <Legacy/stm32_hal_legacy.h>
#include <uart.h>
#include <terminal.h>
#include "system_init.h"
#include "gpio.h"
// Algorithm

void on_b1_pressed();

static void wait_for_button(MouseController *mouse);

int main() {

    // initialize clock and system configuration
    system_init();

    // Initialize all configured peripherals
    peripheral_init();

    // ADC
    // This is A5 Pin in Nucleo-64
    // adc_init_t adc = {
    //         .device = RECV_ADC,
    //         .channel = KB_ADC_CH10
    // };
    // adc_init(&range_front_right, &adc);
    // adc_pin(RECV_FR_PORT, RECV_FR_PIN);

    // Set interrupt button
    gpio_init_t gpioInit{};
    gpioInit.Mode = GPIO_MODE_INPUT;
    gpioInit.Pull = NOPULL;
    gpio_isr_enable(B1_PORT, B1_PIN, &gpioInit, FALLING_EDGE);
    gpio_isr_register(B1_PORT, B1_PIN, on_b1_pressed);


//     Set toggling pin controlled by the button
//     This pin controls the infared LED.
//     This is A0 pin in Nucleo-64
    gpio_init_t gpioInit2;
    gpioInit2.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInit2.Pull = GPIO_NOPULL;
    gpioInit2.Speed = GPIO_SPEED_LOW;
    gpio_init(LED1_PORT, LED1_PIN, &gpioInit2);

    int s = terminal_init();

    while (1) {
    }

}


void on_b1_pressed() {
    gpio_set(LED1_PORT, LED1_PIN, GPIO_PIN_SET);
    HAL_Delay(500);
    gpio_set(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);
    HAL_Delay(500);
    gpio_set(LED1_PORT, LED1_PIN, GPIO_PIN_SET);
    HAL_Delay(500);
    gpio_set(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);
    terminal_printf("one\n");
}
