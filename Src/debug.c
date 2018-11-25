#include "main.h"

struct __FILE {
    int dummy;
};

FILE __stdout;

int fputc(int ch, FILE *f)
{
	usartSendData(USART2, (uint8_t) ch);
    
	while(usartGetFlagStatus(USART2, USART_FLAG_TC) == RESET);
   
	return ch;
}
