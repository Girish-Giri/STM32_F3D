#include "stm32f3xx.h"
#include "LCD.h"
#include "DELAY.h"

uint16_t samples[2] = {0, 0}; //store data in array

void configurepin(void){
	RCC-> AHBENR |= (1<<17) | (1<<22);	//io-port A & F RCC enable
	//here ADC channels 3, 5 are used.
	//set GPIO moder register to AF MODE (GPIO A2, F4 has ADC1_IN3, ADC1_IN5 respectively):
	GPIOA-> MODER |=  (GPIO_MODER_MODER2_0) | (GPIO_MODER_MODER2_1);//set moder to make 00 to 11
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
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;	//Enable DMA1
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
	ADC1->SMPR1 |= ADC_SMPR1_SMP3_0 | ADC_SMPR1_SMP3_1 | ADC_SMPR1_SMP3_2; //set smpl rate of chnl 3
	ADC1->SMPR1 |= ADC_SMPR1_SMP5_0 | ADC_SMPR1_SMP5_1 | ADC_SMPR1_SMP5_2; //set smpl rate of chnl 5
	ADC1->SQR1 |= ADC_SQR1_L_0;	//Set scanning sequence for 2 channels
	ADC1->SQR1 |= (0x05 << 12) | (0X03 << 6);  //Set chnl 3 and 5 to squnce 1, 2 respectively
	ADC1->CFGR |= ADC_CFGR_CONT; 	// ADC ContinuousConvMode Enable

	ADC1->CFGR |= ADC_CFGR_DMAEN; 	//enable ADC DMA
	ADC1->CFGR |= ADC_CFGR_DMACFG;	//Set in circular scan mode

	//dma setup
	DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));	//take data
	DMA1_Channel1->CMAR = (uint32_t)samples;		//store in array
	DMA1_Channel1->CNDTR = 2;						//set for 2 channel
	DMA1_Channel1->CCR |= DMA_CCR_CIRC | DMA_CCR_MINC | DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR_EN;

	//not to be in while loop since set in continuous mode already (line 49)
	ADC1->CR |= ADC_CR_ADSTART;			 //Start conversion
	while((ADC1->ISR & ADC_ISR_EOC) == 0)	//wait until end of conversion
	{
	}

	LCDSetCursorLocation(0, 0);
	LCDSendString("ADCin3:");
	LCDSetCursorLocation(64, 1);
	LCDSendString("ADCin5:");

  while (1)
  {
		LCDSetCursorLocation(8, 0);
		LCDSendInteger(samples[0], 4);
		LCDSetCursorLocation(72, 1);
		LCDSendInteger(samples[1], 4);
  }

  return 0;
}
