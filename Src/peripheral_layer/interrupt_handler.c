#include "common.h"
#include "interrupt_handler.h"
#include "tick.h"

#ifndef USE_FREERTOS

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

void DebugMon_Handler(void) {
}

#endif

//void PendSV_Handler(void) {
//}
//
//void SVC_Handler(void) {
//}

#endif

#ifdef USE_FREERTOS
extern void xPortSysTickHandler(void);
#endif

/**
 * This function is vital important, don't delete it because delay_us() and
 * overrided HAL_delay() must use systick to caculate time delay.
 */
void SysTick_Handler(void) {
    tick_inc_ms();

#ifdef USE_FREERTOS
    // FreeRTOS Tick handler
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xPortSysTickHandler();
    }
#endif
}
