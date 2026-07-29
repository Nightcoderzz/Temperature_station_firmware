/*
 * timers.h
 *
 *  Created on: 12 Jul 2026
 *      Author: sikor
 */

#ifndef TIMERS_H_
#define TIMERS_H_

void timer_init(void);

static inline void delay_us(uint16_t us)
{
    uint16_t start = TIM3->CNT;
    while ((uint16_t)(TIM3->CNT - start) < us);
}

static inline void delay_ms(uint16_t ms)
{
    while (ms--) delay_us(1000);
}

#endif
