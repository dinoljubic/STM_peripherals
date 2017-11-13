// timer.h

#ifndef TIMER_H
#define TIMER_H

//#include "stm32f4xx.h"                  // Device header
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include <main.h>
#include <gpio.h>

void timer2_init(void);
void timer3_pwm_init(void);
void timer2_start(void);
uint32_t timer2_get_milisec(void);
void timer2_wait_millisec(uint32_t ms);



#endif
