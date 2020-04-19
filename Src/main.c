#include "main.h"

int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	TM_GPIO_InitAlternate(GPIOA, GPIO_Pin_2 | GPIO_Pin_3, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_USART2);
	
	usartInit(USART2, BAUD_RATE);
	
	printf("Bootloader. Jumping to user application...\n");
	bootloaderJump();
	
}
