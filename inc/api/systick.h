// systick.h

#ifndef SYSTICK_H
#define SYSTICK_H

#ifndef DELAY_MS
#define DELAY_MS	1000		//default delay interval
#endif
 
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <gpio.h>
#include <main.h>


void SysTick_init(void);
void SysTick_Handler(void);

#endif
