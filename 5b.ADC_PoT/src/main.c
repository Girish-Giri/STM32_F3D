#include "stm32f3xx.h"
#include "LCD.h"
#include "DELAY.h"

void configurepin(void){
	RCC-> AHBENR |= (1<<17) | (1<<22);	//io-port A & F RCC enable
	//here ADC channel 5 is used.
	//set GPIO moder register to ANALOG MODE (GPIO F4 has ADC1_IN5):
	GPIOF-> MODER |=  (GPIO_MODER_MODER4_0) | (GPIO_MODER_MODER4_1);//set moder to make 00 to 11
}


int main(void)
{
	LCDInitializePort();
	LCDSetup();

	//Select Clock for ADC
	RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV6;	// Configure the ADC clock, not to exceed 14Mhz So 72Mhz/6
	RCC->AHBENR |= RCC_AHBENR_ADC12EN;	// Enable ADC1 clock
	ADC1_2_COMMON->CCR |=  ADC12_CCR_CKMODE; //IMP, Connect clock to ADC1
	configurepin();

	//ADC Calibration
	ADC1->CR |= (ADC_CR_ADCAL);		//Calibrate
	while((ADC1->CR & ADC_CR_ADCAL) != 0)
	{
	}

	//ADC Enable
	ADC1->CR |= (ADC_CR_ADEN);					//Enable ADC1
	while((ADC1->ISR & ADC_ISR_ADRD) == 0)
	{
	}
	ADC1->SMPR1 |= ADC_SMPR1_SMP5_0 | ADC_SMPR1_SMP5_1 | ADC_SMPR1_SMP5_2; //set smpl rate of chnl 5
	ADC1->SQR1 |= (0X05 << 6);	     //Set chnl 5 to squnce 1
	ADC1->CFGR |= ADC_CFGR_CONT; 	// ADC ContinuousConvMode Enable

	LCDSetCursorLocation(0, 0);
	LCDSendString("ADCin5:");
  while (1)
  {
	ADC1->CR |= ADC_CR_ADSTART;			 //Start conversion
	while((ADC1->ISR & ADC_ISR_EOC) == 0)	//wait until end of conversion
	{
	}
	LCDSetCursorLocation(8, 0);
	LCDSendInteger(ADC1->DR, 4);
  }

  return 0;
}



