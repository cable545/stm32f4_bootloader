#include "main.h"

/*!< USART CR1 register clear Mask ((~(uint16_t)0xE9F3)) */
#define CR1_CLEAR_MASK            ((uint16_t)(USART_CR1_M | USART_CR1_PCE | \
                                              USART_CR1_PS | USART_CR1_TE | \
                                              USART_CR1_RE))
																							
/*!< USART CR3 register clear Mask ((~(uint16_t)0xFCFF)) */
#define CR3_CLEAR_MASK            ((uint16_t)(USART_CR3_RTSE | USART_CR3_CTSE))							

void usartInit(USART_TypeDef* USARTx, uint32_t bautRate)
{
	uint32_t tmpreg = 0x00, apbclock = 0x00;
	uint32_t integerdivider = 0x00, fractionaldivider = 0x00;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	
	tmpreg = USARTx->CR2;
  tmpreg &= (uint32_t)~((uint32_t)USART_CR2_STOP);
  tmpreg |= (uint32_t)USART_StopBits_1; 
  USARTx->CR2 = (uint16_t)tmpreg;
	
	tmpreg = USARTx->CR1;
  tmpreg &= (uint32_t)~((uint32_t)CR1_CLEAR_MASK);
  tmpreg |= (uint32_t)USART_WordLength_8b | USART_Parity_No | USART_Mode_Rx | USART_Mode_Tx;
  USARTx->CR1 = (uint16_t)tmpreg;
	
	tmpreg = USARTx->CR3;
  tmpreg &= (uint32_t)~((uint32_t)CR3_CLEAR_MASK);
  tmpreg |= USART_HardwareFlowControl_None;
  USARTx->CR3 = (uint16_t)tmpreg;
	
	/* Configure the USART Baud Rate */
  RCC_GetClocksFreq(&RCC_ClocksStatus);

  if((USARTx == USART1) || (USARTx == USART6))
  {
    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  }
  else
  {
    apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  }
  
  /* Determine the integer part */
  if ((USARTx->CR1 & USART_CR1_OVER8) != 0)
  {
    /* Integer part computing in case Oversampling mode is 8 Samples */
    integerdivider = ((25 * apbclock) / (2 * bautRate));    
  }
  else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
  {
    /* Integer part computing in case Oversampling mode is 16 Samples */
    integerdivider = ((25 * apbclock) / (4 * bautRate));    
  }
  tmpreg = (integerdivider / 100) << 4;

  /* Determine the fractional part */
  fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

  /* Implement the fractional part in the register */
  if ((USARTx->CR1 & USART_CR1_OVER8) != 0)
  {
    tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
  }
  else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
  {
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
  }
  
  USARTx->BRR = (uint16_t)tmpreg;
	
	usartEnable(USARTx);
}

void usartSendData(USART_TypeDef* USARTx, uint16_t data)
{
  USARTx->DR = (data & (uint16_t)0x01FF);
}

/*
 * Checks whether the specified USART flag is set or not.
 * USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or UART peripheral.
 * USART_FLAG: specifies the flag to check.
 *  This parameter can be one of the following values:
 *    USART_FLAG_CTS:  CTS Change flag (not available for UART4 and UART5)
 *    USART_FLAG_LBD:  LIN Break detection flag
 *    USART_FLAG_TXE:  Transmit data register empty flag
 *    USART_FLAG_TC:   Transmission Complete flag
 *    USART_FLAG_RXNE: Receive data register not empty flag
 *    USART_FLAG_IDLE: Idle Line detection flag
 *    USART_FLAG_ORE:  OverRun Error flag
 *    USART_FLAG_NE:   Noise Error flag
 *    USART_FLAG_FE:   Framing Error flag
 *    USART_FLAG_PE:   Parity Error flag
 * Returns the new state of USART_FLAG (SET or RESET).
 */
FlagStatus usartGetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG)
{
  FlagStatus bitstatus = RESET;
    
  if((USARTx->SR & USART_FLAG) != (uint16_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

void usartEnable(USART_TypeDef* USARTx)
{ 
	/* Enable the selected USART by setting the UE bit in the CR1 register */
  USARTx->CR1 |= USART_CR1_UE;
}

void usartDisable(USART_TypeDef* USARTx)
{
	/* Disable the selected USART by clearing the UE bit in the CR1 register */
	USARTx->CR1 &= (uint16_t)~((uint16_t)USART_CR1_UE);
}


