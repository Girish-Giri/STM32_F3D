#include "stm32f3xx.h"
#include "LCD.h"

void Delay(int del)
{
	int j=0;
	for (int i = 0; i <= del; ++i)
	{
		j++;
	}
}


int main(void)
{
	LCDInitializePort();
	LCDSetup();

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;		//Turn on timer 1 clock
	TIM1->CR1 |= TIM_CR1_CEN | TIM_CR1_DIR | TIM_CR1_CMS_0;	//Turn on timer 1, set direction to upcount,
	TIM1->PSC = 65535;						//set pre-scalar, to decide no. pulses to be skipped
	TIM1->ARR = 1000;					//set limit of count in auto reload register

	LCDSend2LinesWithDelay(0, "COUNTER:        ", "PRESCLR:");
	LCDSetCursorLocation(73, 1);	//next line starts from 64
	LCDSendInteger(TIM1->PSC, 5);

  while (1)
  {
	  LCDSetCursorLocation(9, 0);
	  LCDSendInteger(TIM1->CNT, 5);	//display count
	  Delay(100000);
	  LCDSetCursorLocation(9, 0);
	  LCDSendString("     ");
  }

  return 0;
}
