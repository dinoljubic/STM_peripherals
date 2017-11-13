// gpio.c

#include <gpio.h>

void gpio_init()
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	//init LEDs
	RCC_AHB1PeriphClockCmd(LED3_GREEN_RCC_GPIOx, ENABLE);
	GPIO_InitStruct.GPIO_Pin		= LED3_GREEN_PinNumber;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init(LED3_GREEN_GPIOx, &GPIO_InitStruct);
	
	RCC_AHB1PeriphClockCmd(LED4_RED_RCC_GPIOx, ENABLE);
	GPIO_InitStruct.GPIO_Pin		= LED4_RED_PinNumber;
	GPIO_Init(LED4_RED_GPIOx, &GPIO_InitStruct);
	
	//init button
	RCC_AHB1PeriphClockCmd(BUTTON_RCC_GPIOx, ENABLE);
	GPIO_InitStruct.GPIO_Pin		= BUTTON_PinNumber;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd		= GPIO_PuPd_NOPULL;//
	GPIO_Init(BUTTON_GPIOx, &GPIO_InitStruct);
	
	//zamjeniti defineovima
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	//inicijalizacije interrupta maknuti negdje dalje?
	
	
}

void gpio_led_state(uint8_t LED_ID, uint8_t state)
{
	BitAction bitValue;
	
	bitValue = (state == 0) ? Bit_SET : Bit_RESET;
	switch(LED_ID)
	{
		case LED3_GREEN_ID:
			GPIO_WriteBit(LED3_GREEN_GPIOx, LED3_GREEN_PinNumber, bitValue);
			break;
		case LED4_RED_ID:
			GPIO_WriteBit(LED4_RED_GPIOx, LED4_RED_PinNumber, bitValue);
			break;
		
	}
}

void gpio_led_toggle(uint8_t LED_ID)
{
	switch(LED_ID)
	{
		case LED3_GREEN_ID:
			GPIO_ToggleBits(LED3_GREEN_GPIOx, LED3_GREEN_PinNumber);
			break;
		case LED4_RED_ID:
			GPIO_ToggleBits(LED4_RED_GPIOx, LED4_RED_PinNumber);
			break;
	}
}
