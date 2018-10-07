#include <mouse_controller.h>
#include <system_init.h>
#include <adc.h>
#include "trace.h"
#include "tick.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


static void on_b1_pressed(void *args);

static void task_blinky(void *pvParameters);


/* Task Handlers */
TaskHandle_t task_blinky_handler;
TaskHandle_t task_main_handler;
static SemaphoreHandle_t b1_semphr = NULL;
static SemaphoreHandle_t one_clikc_semphr = NULL;
static SemaphoreHandle_t double_click_semphr = NULL;

static void wait_for_button(MouseController *mouse);

int main(void) {
    // initialize clock and system configuration
    system_init();

    // Initialize all configured peripherals
    peripheral_init();

    // Initialize terminal printf
    terminal_init();

    /* Set ADC, this is A5 Pin in Nucleo-64 */
//    adc_init_t adc_setting{};
//    adc_setting.device = MM_ADC1;
//    adc_setting.channel = MM_ADC_CH5;
//    adc_setting.adc_port = PORTA;
//    adc_setting.adc_pin = PIN_5;
//    adc_t adc_handler{};
//    MM_DEBUG_MSG("adc init\n");
//    if (adc_init(&adc_handler, &adc_setting) != MM_OK) {
//        MM_DEBUG_MSG("adc init error\n");
//    }
//    MM_DEBUG_MSG("adc init done\n");


    /* Set interrupt button */
    gpio_init_t button_pin_setting{};
    button_pin_setting.Mode = GPIO_MODE_INPUT;
    button_pin_setting.Pull = NOPULL;
    gpio_irt_enable(B1_PORT, B1_PIN, &button_pin_setting, FALLING_EDGE);
    gpio_irt_register(B1_PORT, B1_PIN, on_b1_pressed, NULL);

    b1_semphr = xSemaphoreCreateCounting(1, 0);
    one_clikc_semphr = xSemaphoreCreateCounting(1, 0);
    double_click_semphr = xSemaphoreCreateCounting(1, 0);


    BaseType_t result;
    /* definition and creation of defaultTask */
    result = xTaskCreate(
            task_blinky,            /* Pointer to the function that implements the task */
            "Blinky",               /* Text name for the task. This is to facilitate debugging only. It is not used in the scheduler */
            configMINIMAL_STACK_SIZE, /* Stack depth in words */
            NULL,                   /* Pointer to a task parameters */
            configMAX_PRIORITIES -
            1,                      /* The task priority */
            &task_blinky_handler);  /* Pointer of its task handler, if you don't want to use, you can leave it NULL */

    if (result != pdPASS) {
        MM_DEBUG_ERROR("Creating task failed!!");
    }

    /**
     * Do not put delay function in this section!
     * Because xTaskCreate will stop systick until the scheduler called
     */

    /* Start scheduler */
    vTaskStartScheduler();
    /* It never goes here, but the task should be deleted when it reaches */
    while (1) {
    }
}


/* vBlinkyTask function */
void task_blinky(void *pvParameters) {
    portTickType xLastWakeTime;
    /* Initialize xLastWakeTime for vTaskDelayUntil */
    /* This variable is updated every vTaskDelayUntil is called */
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        // Wait for the first pressing.
        xSemaphoreTake(b1_semphr, portMAX_DELAY);
        gpio_set(LED1_PORT, LED1_PIN, GPIO_PIN_SET);

        // wait for the second until 500 ms
        if (pdTRUE == xSemaphoreTake(b1_semphr, (1000 / portTICK_RATE_MS))) {
            xSemaphoreGive(double_click_semphr);
        } else {
            xSemaphoreGive(one_clikc_semphr);
        }
        gpio_set(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);

        /* Call this Task explicitly every 10ms ,NOT Delay for 10ms */
        vTaskDelayUntil(&xLastWakeTime, (10 / portTICK_RATE_MS));
    }

    /* It never goes here, but the task should be deleted when it reaches here */
    vTaskDelete(NULL);
}


/*******************************************************************************
 * Event handlers
 ******************************************************************************/
void on_b1_pressed(void *args) {

    if (b1_semphr != NULL) {
        xSemaphoreGiveFromISR(b1_semphr, NULL);
    }
}