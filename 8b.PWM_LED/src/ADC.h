/*
 * ADC.h
 *
 *  Created on: 29-Oct-2020
 *      Author: GIRISH
 */

#ifndef ADC_H_
#define ADC_H_

#define c1 0x00


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
}


void ADCChannel(uint8_t channel)
{
	switch(channel)
	    {
	        case 1:
	            ADC1->SQR1 |= 0x01 << 6;
	            break;

	        case 2:
	            ADC1->SQR1 |= 0x02 << 6;
	            break;

	        case 3:
	            ADC1->SQR1 |= 0x03 << 6;
	            break;

	        case 4:
	            ADC1->SQR1 |= 0x04 << 6;
	            break;

	        case 5:
	            ADC1->SQR1 |= 0x05 << 6;
	            break;

	        case 6:
	            ADC1->SQR1 |= 0x06 << 6;
	            break;

	        case 7:
	            ADC1->SQR1 |= 0x07 << 6;
	            break;

	        case 8:
	            ADC1->SQR1 |= 0x08 << 6;
	            break;

	        case 9:
	            ADC1->SQR1 |= 0x09 << 6;
	            break;

	        case 10:
	            ADC1->SQR1 |= 0x0A << 6;
	            break;

	        case 11:
	            ADC1->SQR1 |= 0x0B << 6;
	            break;

	        case 12:
	            ADC1->SQR1 |= 0x0C << 6;
	            break;

	        case 13:
	            ADC1->SQR1 |= 0x0D << 6;
	            break;

	        case 14:
	            ADC1->SQR1 |= 0x0E << 6;
	            break;

	        case 15:
	            ADC1->SQR1 |= 0x0F << 6;
	            break;

	        case 16:						//IMP: STARTS CONVERSION ON CHNL 16
	            ADC1->SQR1 |= 0x10 << 6;
	            break;

	        case 17:
	            ADC1->SQR1 |= 0x11 << 6;
	            break;

	        case 18:
	            ADC1->SQR1 |= 0x12 << 6;
	            break;

	        // operator doesn't match any case constant +, -, *, /
	        default:
	            printf("Error! channel no. is not correct");
	    }
}


void ADCRun()
{
	 ADC1->CR |= ADC_CR_ADSTART;			 //Start conversion
	 while((ADC1->ISR & ADC_ISR_EOC) == 0)	//wait until end of conversion
	 {
	 }
}



#endif /* ADC_H_ */
