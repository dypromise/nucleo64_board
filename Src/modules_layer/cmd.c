/*
 * motion_controller.c
 *
 *  Created on: Apr 8, 2017
 *      Author: kbumsik
 */

#include <stdio.h>
#include "cmd.h"
#include "pid.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/*******************************************************************************
 * Private variables and functions
 ******************************************************************************/
static QueueHandle_t cmd_queue = NULL;
static SemaphoreHandle_t cmd_semphr = NULL;

/*******************************************************************************
 * Function definition
 ******************************************************************************/
void cmd_init(void) {
    // /* Get queue from the controll loop */
    cmd_queue = thread_control_loop_cmd_queue();
    cmd_semphr = thread_control_loop_cmd_semphr();
    if (cmd_queue == NULL) {
        MM_DEBUG_ERROR("Getting cmd queue failed!!");
    }
}

void cmd_low_pid_and_go(struct cmd_pid *cmd, struct cmd_events *events) {
    // process events
    if (events != NULL) {
        if (events->on_start != NULL) {
            events->on_start();
        }
    }
    // int32_t step_left;  //< if it is zero, not target step
    // int32_t step_right; //< if it is zero, not target step
    struct cmd_queue_element commend = {
            .type = CMD_LOW_PID_AND_GO,
            .pid = *cmd,
    };
    if (xQueueSend(cmd_queue, &commend, portMAX_DELAY) != pdPASS) {
        // failed to send a commend
    }
    // process events
    if (events != NULL) {
        if (events->on_completed != NULL) {
            events->on_completed();
        }
    }
}

void cmd_low_pid_reset_and_stop(struct cmd_events *events) {
    // process events
    if (events != NULL) {
        if (events->on_start != NULL) {
            events->on_start();
        }
    }

    struct cmd_queue_element commend = {
            .type = CMD_LOW_PID_RESET_AND_STOP,
    };
    if (xQueueSend(cmd_queue, &commend, portMAX_DELAY) != pdPASS) {
        // failed to send a commend
    }

    // process events
    if (events != NULL) {
        if (events->on_completed != NULL) {
            events->on_completed();
        }
    }
}

void cmd_polling(enum cmd_type type) {
    struct cmd_queue_element commend = {
            .type = type,
    };
    if (xQueueSend(cmd_queue, &commend, portMAX_DELAY) != pdPASS) {
        // failed to send a commend
    }
    // Wait for semaphore
    xSemaphoreTake(cmd_semphr, portMAX_DELAY);
}
