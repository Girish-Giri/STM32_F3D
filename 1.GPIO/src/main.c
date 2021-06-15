#include "stm32f3xx.h"

void msdelay(int delay)
{
	volatile int i, j;
	for (i = 0; i < delay; ++i) {
		++j;
	}
}

void configurepin(void){
	RCC-> AHBENR |= (1<<19);	//io-port c RCC enable
	//set gpioc mode register to GPIO MODE:
	GPIOC-> MODER |=  (GPIO_MODER_MODER6_0);	//set moder to make lsb 1 of 00
	GPIOC-> MODER &= ~ (GPIO_MODER_MODER6_1);	//set moder to make msb 0 of 00
}

int main(void)
{
	configurepin();

  while (1)
  {
	  GPIOC->ODR |= (GPIO_ODR_6);	//set bit of pin6
	  msdelay(100000);
	  GPIOC->ODR &= ~(GPIO_ODR_6);	//reset bit of pin6
	  msdelay(100000);
  }


}
