	#include <exti.h>
	
void exti_init(void)
{	
	
	NVIC_InitTypeDef	NVIC_InitStruct;
	EXTI_InitTypeDef	EXTI_InitStruct;
	
	
	EXTI_InitStruct.EXTI_Line		= EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode		= EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		gpio_led_toggle(1);
		gpio_led_toggle(2);
		timer2_start();
	}
}
