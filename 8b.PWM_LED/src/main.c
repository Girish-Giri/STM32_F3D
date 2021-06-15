#include "stm32f3xx.h"
#include "LCD.h"
#include "ADC.h"

void Delay(int del)
{
	int j=0;
	for (int i = 0; i <= del; ++i)
	{
		j++;
	}
}

void configurepin(void){
	RCC-> AHBENR |= (1<<22);	//io-port F RCC enable
	//here channel for adc chnged since port A used in AF MODE required for PWM utility
	//set GPIO mode register to ANALOG MODE (GPIO F4 has ADC1_IN5):
	GPIOF-> MODER |=  (GPIO_MODER_MODER4_0) | (GPIO_MODER_MODER4_1);//set moder to make 00 to 11
}

int main(void)
{
	LCDInitializePort();
	LCDSetup();
	ADCInitialize();
	ADCChannel(5);
	configurepin();

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	//Turn on timer 1 clock
	RCC-> AHBENR |= RCC_AHBENR_GPIOAEN; //Enable port A bus
	//set GPIO moder register to AF MODE (GPIO A8 has TIM1_CH1):
	GPIOA->MODER &= ~(GPIO_MODER_MODER8_0);	//set moder to make lsb to 0 of 10
	GPIOA->MODER |= GPIO_MODER_MODER8_1;	//set moder to make msb to 1 of 10
	GPIOA->AFR[1] = 0x06;					//GPIO_AFRH_AFRH1 | GPIO_AFRH_AFRH2;

	TIM1->PSC = 0;					//set pre-scalar, to decide no. pulses to be skipped
	//4095 since 12 bit ADC used:
	TIM1->ARR = 4095;				//set limit of count (pwm period) in auto reload register
	TIM1->CCR1 = 2000;				//set duty cycle
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE; 	//set to 0110 for PWM1 mode
	TIM1->CCER |= TIM_CCER_CC1E;	//cc1 configured as o/p
	TIM1->BDTR |= TIM_BDTR_MOE;		//main o/p enable
	TIM1->CR1 |= TIM_CR1_CEN | TIM_CR1_DIR;	//Turn on timer 1
	TIM1->EGR |= TIM_EGR_UG;	//for o/p


	LCDSend2LinesWithDelay(0, "ADC:            ", "                ");

  while (1)
  {
	  ADCRun();
	  LCDSetCursorLocation(4, 0);
	  LCDSendString("     ");
	  LCDSetCursorLocation(4, 0);
	  LCDSendInteger(ADC1->DR, 4);
	  TIM1->CCR1 = ADC1->DR;
	  //Delay(70000);
  }

  return 0;
}
