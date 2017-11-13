#include <main.h>

int main(void)
{
	char c;
	
	USART3_Init();
	
	/*for (uint8_t i = 0; i < 255; i++)
		USART1_SendChar(i);*/
	
	while(1)
	{
		if (USART3_Dequeue(&c) != 0)
			USART3_SendChar(c);
	}
}
