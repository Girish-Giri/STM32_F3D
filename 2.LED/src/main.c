#include "stm32f3xx.h"

void WaitForAMoment(int moment)			//Time delay function
	{
		volatile int i, j;
		for (i = 0; i < moment; ++i)
		{
			j++;
		}
	}
void configureledpins(void)				//Register Setup function
	{
		RCC->AHBENR |= (1<<21);			//IOPE AH BUS ENABLE
		GPIOE->MODER &= ~(0xFFFF<<8*2);	// 0000 0000 0000 0000 1111 1111 1111 1111
		GPIOE->MODER |= (0x5555<<8*2);	// 0000 0000 0000 0000 0101 0101 0101 0101
		//GPIO MODER SET TO 01 TO MAKE IT GPIO
	}

int main(void)
{
	configureledpins();

  while (1)
  {
	 //Bitwise XORing
	 GPIOE->ODR ^=(0x55 << 8);		//0000 0000 0000 0000 0000 0000 0101 0101
	 WaitForAMoment(30000);
	 GPIOE->ODR ^=(0xAA << 8);		//0000 0000 0000 0000 0000 0000 1010 1010
	 WaitForAMoment(30000);
  }
}


