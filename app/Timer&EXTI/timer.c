#include <timer.h>

/*
programsko brojanje milisekundi ne radi; na prvom IRQ it TIM2 zastavica se ne gasi
i odmah se okida svih potrebnih 1000 
*/
#define DELAY_MS	3000

uint32_t timer2_Ticks_Millisec;

void timer2_init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	NVIC_InitTypeDef					NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
	TIM_ITConfig (TIM2, TIM_IT_Update, DISABLE);
	
	/*
	HCLK	= 180MHz
	PCLK1	=	HCLK / 4 = 45MHz
	-TIM2CLK = 2*PCLK, if HCLK-PCLK1 prescaler is not 1
	TIM2CLK = 90MHz
	prescaler = 3 ->30MHz
	desired output frequency: 1kHz
	Period = TIM2CLK/freq = 30000 = 0x7530
	*/
	
	
	//time base
	TIM_TimeBaseStructure.TIM_Prescaler = 2000;	//kompenzacija za ono sto ne radi
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0x7530;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//TIM2 IRQ
	TIM_Cmd(TIM2, DISABLE);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	//TIM2 global interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//global var
	timer2_Ticks_Millisec = 0;

	//counter enable
	//TIM_Cmd(TIM2, ENABLE);
	
}

void timer2_start(void)
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)			//kod pokretanja brojaca zastavica pending
	{																												//IRQ dize se u 1 i ova provjera prolazi
		if (++timer2_Ticks_Millisec == DELAY_MS)							//inkrement je u provjeri (koja ide svaki put)
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
