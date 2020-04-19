#include "main.h"

typedef void (*pFunction)(void);

void bootloaderJump(void)
{
	uint32_t jumpAddress = *(__IO uint32_t*)(APP_ADDRESS + 4);
	pFunction AppJump = (pFunction)jumpAddress;
	
	RCC_DeInit();
	
	/**
	 * Step: Disable systick timer and reset it to default values
	 */
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	
	__disable_irq();
	
	SCB->VTOR = APP_ADDRESS;
	
//	SYSCFG->MEMRMP = 0x01;
	
	__set_MSP(*(uint32_t *)APP_ADDRESS);
	
	AppJump();
}
