#include <main.h>

int main()
{
	SystemInit();
	gpio_init();
	gpio_led_state(1, 1);
	gpio_led_state(2, 0);
	SysTick_init();
	
	while(1);
}
