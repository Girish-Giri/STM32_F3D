#include "stm32f3xx.h"

void configurepins(void)
	{
		RCC->AHBENR |= (RCC_AHBENR_GPIODEN);		//IOPORT C AH BUS ENABLE FOR LED OUT
		RCC->AHBENR |= (RCC_AHBENR_GPIOAEN);		//IOPORT A AH BUS ENABLE FOR SW PIN
		GPIOD->MODER |= (GPIO_MODER_MODER6_0);		//MODER OF OUT LSB 1
		GPIOD->MODER &= ~(GPIO_MODER_MODER6_1);		//MODER OF OUT MSB 0
		GPIOA->MODER &= ~(GPIO_MODER_MODER0);		//MODER OF IN MSB+LSB 00
	}

int main(void)
{
	configurepins();

  while (1)
  {
	  if (GPIOA->IDR & GPIO_IDR_0)		//&ing BIT WISE IDRegistor with 0X00000001, if button pressed: TRUE
	  {
		  GPIOD->ODR |= (GPIO_ODR_6);	//OUT PUT LED PIN HIGH
	  }
	  else
	  {
		  GPIOD->ODR &= ~(GPIO_ODR_6);	//OUT PUT LED PIN LOW
	  }

  }


}
