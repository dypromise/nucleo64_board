#ifndef SYSTEM_MM_COMMON_H_
#define SYSTEM_MM_COMMON_H_

#define MM_MSG_BASE "common"


#ifdef  STM32F4
#include "stm32f4xx.h"
#else
#error  "Something wrong with CMSIS file (stm32f4xx.h). define STM32F4 manually"
#endif


#ifdef MM_DEBUG
#if defined(MM_DEBUG_TO_TERMINAL)
#include "terminal.h"
#define MM_DEBUG_MSG(msg, ...)    terminal_printf("[INFO][In " MM_MSG_BASE ", line %d]: " msg, __LINE__ ,##__VA_ARGS__)
#define MM_DEBUG_WARNING(msg, ...)    terminal_printf("[WARNING][In " MM_MSG_BASE ", line %d]: " msg, __LINE__ , ##__VA_ARGS__)
#define MM_DEBUG_ERROR(msg, ...)    terminal_printf("[ERROR][In " MM_MSG_BASE ", line %d]: " msg, __LINE__ , ##__VA_ARGS__)
#elif defined(MM_DEBUG_TO_SEMIHOSTING)
#include "trace.h"
#define MM_DEBUG_MSG(msg, ...)	trace_printf("[INFO][In " MM_MSG_BASE ", line %d]: " msg, __LINE__ ,##__VA_ARGS__)
#define MM_DEBUG_WARNING(msg, ...)	trace_printf("[WARNING][In " MM_MSG_BASE ", line %d]: " msg, __LINE__ , ##__VA_ARGS__)
#define MM_DEBUG_ERROR(msg, ...)	trace_printf("[ERROR][In " MM_MSG_BASE ", line %d]: " msg, __LINE__ , ##__VA_ARGS__)
#else
#error "Define either DEBUG_TO_SEMIHOSTING or DEBUG_TO_TERMINAL. If you didn't set up Terminal, you may prefer DEBUG_TO_SEMIHOSTING."
#endif
#else
#define MM_DEBUG_MSG(msg, ...)
#define MM_DEBUG_WARNING(msg, ...)
#define MM_DEBUG_ERROR(msg, ...)
#endif

#define TIMEOUT_MAX HAL_MAX_DELAY
#define MM_WRONG_PIN 0xFF
typedef enum {
    MM_OK = HAL_OK,
    MM_ERROR = HAL_ERROR,
    MM_BUSY = HAL_BUSY,
    MM_TIMEOUT = HAL_TIMEOUT,
    MM_UNKNOWN = -4
} status_t;

/*
 * Handler to external (a.k.a manufacturers like ST) library
 * This will be handled in .c source file
 */
typedef void *extlib_handle_t;


// Push button settings
#define B1_PORT        GPIOC
#define B1_PIN         GPIO_PIN_13

#define B2_PORT        GPIOC
#define B2_PIN         GPIO_PIN_12

// LED settings
#define LED1_PORT    GPIOA        //LD2
#define LED1_PIN    GPIO_PIN_5
#define LED2_PORT    GPIOB
#define LED2_PIN    GPIO_PIN_11
#define LED6_PORT    GPIOA
#define LED6_PIN    GPIO_PIN_2






#endif /* SYSTEM_MM_COMMON_H_ */
