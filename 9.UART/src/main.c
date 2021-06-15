#include "stm32f3xx.h"

char temp;

void USART1_IRQHandler(void)
{
	  //check if we are here due to RXNE Interrupt
	  if(USART1->ISR & USART_ISR_RXNE)	//if RXNE not empty
	  {
		  temp = USART1->RDR;	//fetch data received
		  USART1->TDR = temp;	//send it back
		  while(!(USART1->ISR & USART_ISR_TC))	//until Tx completes
		  ;
	  }

	  //check if we are here due to TXE Interrupt
	  if(USART1->ISR & USART_ISR_TXE)
	  {
		  //handle transmit completion here
	  }
}

void configurepin(void){
	RCC-> AHBENR |= (1<<19);	//io-port C RCC enable
	//here USART is used.
	//set GPIO moder register to AF MODE (USART1 PC4-TX, PC5-RX):
	//TX
	GPIOC-> MODER &= ~(GPIO_MODER_MODER4_0);	//set lsb to 0 for 01
	GPIOC-> MODER |= GPIO_MODER_MODER4_1;	//set msb to 1 for 01
	/*//RX
	GPIOC-> MODER &= ~(GPIO_MODER_MODER5_0)	//set lsb to 0 for 01
	GPIOC-> MODER |= GPIO_MODER_MODER5_1;	//set msb to 1 for 01*/
}

int main(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;		//USART 1 CLK ENABLE
	configurepin();
	/*no need to set RX pin in AF mode since RX is a input which is PC5s default state*/
	USART1->BRR |= 0x1D4C;	//Baudrate set to 9600 for 72Mhz on APB2 bus

	//enable RXNE & TXE Interrupts on USART Side
	USART1->CR1 |= USART_CR1_RXNEIE | USART_CR1_TXEIE;	//interrupt bits enabled here

	///******** |  RX enable	|  TX enable   |  UART enable
	USART1->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;

	//interrupt for USART1 on NVIC Side
	NVIC_EnableIRQ(USART1_IRQn);

  while (1)
  {
	  //without using interrupt

	 /* //check if we are here due to RXNE Interrupt
	  if(USART1->ISR & USART_ISR_RXNE)	//if RXNE not empty
	  {
		  temp = USART1->RDR;	//fetch data received
		  USART1->TDR = temp;	//send it back
		  while(!(USART1->ISR & USART_ISR_TC))	//until Tx completes
		  ;
	  }*/
  }

  return 0;
}
