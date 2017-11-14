#include <systick.h>

uint32_t SysTick_ms;

void SysTick_init(void)
{
	SysTick_ms = 0;
	
	SysTick_Config(SystemCoreClock / 1000);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	
}

void SysTick_Handler(void)
{
	if (++SysTick_ms == DELAY_MS)
	{
		SysTick_ms = 0;
		gpio_led_toggle(1);
		gpio_led_toggle(2);
	}
}
