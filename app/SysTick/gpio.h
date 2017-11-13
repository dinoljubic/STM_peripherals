// gpio.h

#ifndef GPIO_H
#define GPIO_H

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

#define LED3_GREEN_RCC_GPIOx		RCC_AHB1Periph_GPIOG
#define LED4_RED_RCC_GPIOx			RCC_AHB1Periph_GPIOG

#define	LED3_GREEN_GPIOx				GPIOG
#define	LED4_RED_GPIOx					GPIOG

#define LED3_GREEN_PinNumber		GPIO_Pin_13
#define LED4_RED_PinNumber			GPIO_Pin_14

#define LED3_GREEN_ID						1
#define LED4_RED_ID							2


void gpio_init(void);
void gpio_led_state(uint8_t LED_ID, uint8_t state);
void gpio_led_toggle(uint8_t LED_ID);

#endif
