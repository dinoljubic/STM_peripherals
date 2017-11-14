#include <usart.h>

// FIFO Buffer
char RX_BUFFER[BUFSIZE];
int RX_BUFFER_HEAD, RX_BUFFER_TAIL;
// Tx state flag
uint8_t TxReady;

void USART1_Init(void)
{
	GPIO_InitTypeDef			GPIO_InitStruct;
	USART_InitTypeDef			USART_InitStruct;
	NVIC_InitTypeDef			NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin		=	GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType	=	GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd		=	GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	
	USART_InitStruct.USART_BaudRate	=	BAUDRATE;
	USART_InitStruct.USART_WordLength		=	USART_WordLength_8b;
	USART_InitStruct.USART_StopBits			= USART_StopBits_1;
	USART_InitStruct.USART_Parity				= USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode					= USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);
	
	USART_ITConfig(USART1, USART_IT_TXE, 	DISABLE);
	USART_ITConfig(USART1, USART_IT_TC, 	ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE,	ENABLE);
	
	TxReady = 1;
	RX_BUFFER_HEAD = 0; RX_BUFFER_TAIL = 0;
	
	NVIC_InitStruct.NVIC_IRQChannel		= USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority		= 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority		= 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd		= ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART1, ENABLE);
	
}

void USART1_IRQHandler(void)
{
	uint16_t rx_char;
	static char rx_head;
	
	// Rx event
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit (USART1, USART_IT_RXNE);
		rx_char	= USART_ReceiveData(USART1);
		rx_head	= RX_BUFFER_HEAD + 1;
		if (rx_head == BUFSIZE)
			rx_head = 0;
		if (rx_head != RX_BUFFER_TAIL)
		{
			RX_BUFFER[RX_BUFFER_HEAD]	= rx_char;
			RX_BUFFER_HEAD	= rx_head;
		}
	}
	
	// Tx event
	if (USART_GetITStatus(USART1, USART_IT_TC) == SET)
	{
		USART_ClearITPendingBit (USART1, USART_IT_TC);
		TxReady	= 1;
	}
}


void USART1_SendChar (char c)
{
	while(!TxReady);
	USART_SendData(USART1, (uint16_t) c);
	TxReady = 0;
}

int USART1_Dequeue (char *c)
{
	int ret = 0;
	*c = 0;
	NVIC_DisableIRQ(USART1_IRQn);
	if (RX_BUFFER_HEAD != RX_BUFFER_TAIL)
	{
		*c	= RX_BUFFER[RX_BUFFER_TAIL];
		RX_BUFFER_TAIL++;
		if (RX_BUFFER_TAIL == BUFSIZE)
			RX_BUFFER_TAIL = 0;
		ret	= 1;
	}
	NVIC_EnableIRQ(USART1_IRQn);
	return ret;
}
