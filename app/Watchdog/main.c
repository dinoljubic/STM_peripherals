#include <main.h>

int main (void)
{
	gpio_init();
	gpio_led_state(1,0);
	for (int i = 0; i < 0x002FFFFF; i++);			//delay
	gpio_led_state(1,1);
	
	iwdg_init();
	
	while(1)
	{
		gpio_led_state(2,0);
		for (int i = 0; i < 0x002FFFFF; i++);			//delay
		iwdg_reload();
		gpio_led_state(2,1);
		for (int i = 0; i < 0x002FFFFF; i++);			//delay
	}
}
