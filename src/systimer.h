/**
 * \file systimer.h
 * \author vEmagic (admin@vemagic.com)
 * \brief
 * \version 0.0.1
 * \date 2020-10-31
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SYSTIMER_H
#define SYSTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void Timer0Init(void);
void Timer0Restart(uint16_t ui16MicroSeconds);
bool Timer0IsElapsed(void);
void Timer0Handler(void) __interrupt 1; // 必须声明，否则不会进中断

#define DelayMs(ms) \
do {    \
    uint16_t ui16MilliSeconds = 0; \
    while (++ui16MilliSeconds < ms) { \
        Timer0Restart(1000); \
        while (false == Timer0IsElapsed()); \
    } \
} while (0)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SYSTIMER_H
