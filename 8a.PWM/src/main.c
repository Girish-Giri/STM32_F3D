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

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	//Turn on timer 1 clock
	RCC-> AHBENR |= RCC_AHBENR_GPIOAEN; //Enable port A bus
	//set gpioa mode register to AF MODE (GPIO A8 has TIM1_CH1):
	GPIOA->MODER &= ~(GPIO_MODER_MODER8_0);	//set moder to make lsb to 0 of 10
	GPIOA->MODER |= GPIO_MODER_MODER8_1;	//set moder to make msb to 1 of 10
	GPIOA->AFR[1] = 0x06;						//GPIO_AFRH_AFRH1 | GPIO_AFRH_AFRH2;

	TIM1->PSC = 0;					//set pre-scalar, to decide no. pulses to be skipped
	TIM1->ARR = 20000;					//set limit of count (pwm period) in auto reload register
	TIM1->CCR1 = 20000;					//set duty cycle
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE; 	//set to 0110 for PWM1 mode
	TIM1->CCER |= TIM_CCER_CC1E;	//cc1 configured as o/p
	TIM1->BDTR |= TIM_BDTR_MOE;		//main o/p enable
	TIM1->CR1 |= TIM_CR1_CEN | TIM_CR1_DIR;	//Turn on timer 1
	TIM1->EGR |= TIM_EGR_UG;	//for o/p


	LCDSend2LinesWithDelay(0, "COUNTER:        ", "PRESCLR:");
	LCDSetCursorLocation(73, 1);	//next line starts from 64
	LCDSendInteger(TIM1->PSC, 5);

  while (1)
  {
	  LCDSetCursorLocation(9, 0);
	  LCDSendInteger(TIM1->CNT, 5);
	  LCDSetCursorLocation(9, 0);
	  LCDSendString("     ");
  }

  return 0;
}
