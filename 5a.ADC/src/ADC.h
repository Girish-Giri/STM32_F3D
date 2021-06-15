/*
 * ADC.h
 *
 *  Created on: 29-Oct-2020
 *      Author: GIRISH
 */

#ifndef ADC_H_
#define ADC_H_

void ADCInitialize()
{
	LCDInitializePort();
	LCDSetup();

	//ADC Calibration
	ADC1->CR &= ~(ADC_CR_ADEN);				//Disable ADC1 for calibration
	ADC1->CR |= (ADC_CR_ADCAL);				//Calibrate
	while((ADC1->CR & ADC_CR_ADCAL) != 0)	//wait
	{
	}

	//Select Clock for ADC
	RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV2;	// Configure the ADC clock
	RCC->AHBENR |= RCC_AHBENR_ADC12EN;		// Enable ADC1 clock
	RCC->CR |= RCC_CR_HSION;				//Select clock For ADC1 and start it
	while((RCC->CR & RCC_CR_HSIRDY) == 0)	//Check if clock ready
	{
	}
	ADC1_2_COMMON->CCR |=  ADC12_CCR_CKMODE;//Connect clock to ADC1

	//ADC Enable
	ADC1->CR |= (ADC_CR_ADEN);				//Enable ADC1
	while((ADC1->ISR & ADC_ISR_ADRD) == 0)
	{
	}
	ADC1->SMPR1 |= ADC_SMPR1_SMP1;			//Set sample rate
	ADC1->CFGR |= ADC_CFGR_CONT; 			// ADC ContinuousConvMode Enable
	ADC1->SMPR1 |= ADC_SMPR1_SMP1;				//Set sample rate

}


void ADCChannel(uint8_t channel)
{
	ADC1->SQR1 |= 1 << channel;			//IMP: STARTS CONVERSION ON CHNL 16
}


void ADCRun()
{
	 ADC1->CR |= ADC_CR_ADSTART;			 //Start conversion
	 while((ADC1->ISR & ADC_ISR_EOC) == 0)	//wait until end of conversion
	 {
	 }
}



#endif /* ADC_H_ */
