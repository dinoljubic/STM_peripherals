#ifndef USART_H
#define USART_H

#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_usart.h>

#define BUFSIZE 		16
#define BAUDRATE		115200

void USART3_Init(void);
void USART3_SendChar(char c);
int USART3_Dequeue(char *c);

#endif
