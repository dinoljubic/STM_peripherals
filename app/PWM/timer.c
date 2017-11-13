#include <timer.h>

uint32_t timer2_Ticks_Millisec;

void timer2_init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStruct;
	NVIC_InitTypeDef					NVIC_InitStruct;
	uint16_t									TimerPeriod;
	RCC_ClocksTypeDef					RCC_Clocks;
	uint32_t									APB1_CLK;
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
	TIM_ITConfig (TIM2, TIM_IT_Update, DISABLE);
	
	RCC_GetClocksFreq(&RCC_Clocks);
	APB1_CLK = RCC_Clocks.PCLK1_Frequency;
	
	TimerPeriod = (uint16_t)(((APB1_CLK) / 1000) - 1);
	
	//time base
	TIM_TimeBaseStruct.TIM_Prescaler = 1000;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Period = TimerPeriod;
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
	
	//TIM2 IRQ
	TIM_Cmd(TIM2, DISABLE);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	//TIM2 global interrupt
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//global var
	timer2_Ticks_Millisec = 0;

	//counter enable
	//TIM_Cmd(TIM2, ENABLE);
	
}


void timer3_pwm_init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStruct;
	TIM_OCInitTypeDef					TIM_OCInitStruct;
	GPIO_InitTypeDef					GPIO_InitStruct;
	
	//TIM3 IO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	
	/*
	HCLK	= 180MHz
	PCLK1	=	HCLK / 4 = 45MHz
	If HCLK-PCLK1 prescaler is not 1
		TIM3CLK = 2*PCLK1
		(otherwise TIM3CLK = PCLK1)
	TIM3CLK = 90MHz
	desired frequency: 2kHz
	Period = TIM3CLK/freq = 45000
	duty cycle = 20% = (Pulse/Period) * 100
		Pulse = 2250
	
	ovdje sam izmjerio ~1kHz tako da nesto ne valja, ali step je 20% perioda
	*/
	
	//Time base
	TIM_TimeBaseStruct.TIM_Period = 45000;
	TIM_TimeBaseStruct.TIM_Prescaler = 0;
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);
	
	//CH1 in PWM mode
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 9000;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
}



void timer2_start(void)
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		if (++timer2_Ticks_Millisec == DELAY_MS)
		{
			//disable timer, clear pending irq bit and reset millisecond counter
			TIM_Cmd(TIM2, DISABLE);
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			timer2_Ticks_Millisec = 0;
			//toggle LEDs
			gpio_led_toggle(1);
			gpio_led_toggle(2);
		}
	}
}

uint32_t timer2_get_millisec()
{
	uint32_t value;
	NVIC_DisableIRQ(TIM2_IRQn);
	value = timer2_Ticks_Millisec;
	NVIC_EnableIRQ(TIM2_IRQn);
	return value;
}

void timer2_wait_millisec(uint32_t ms)
{
	uint32_t t1, t2;
	t1 = timer2_get_millisec();
	while(1)
	{
		t2 = timer2_get_millisec();
		if ((t2 - t1) >= ms) break;
		if (t2 < t1) break;
	}
}
