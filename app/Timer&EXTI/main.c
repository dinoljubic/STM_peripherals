#include <main.h>

int main(void)
{

	gpio_init();
	timer2_init();
	exti_init();
	
	gpio_led_state(1,0);
	gpio_led_state(2,1);
	
	while(1);
		
}


