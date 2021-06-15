#include "stm32f3xx.h"
#include "LCD.h"

#define TEMP110_CAL_ADDR ( (uint16_t*) ( (uint32_t) 0x1FFFF7C2 ) )
#define TEMP30_CAL_ADDR ( (uint16_t*) ( (uint32_t) 0x1FFFF7B8 ) )
#define AVGSLOPE 4.3
#define V25 1.43

float temp = 0;
int main(void)
{
	LCDInitializePort();
	LCDSetup();

	//ADC Calibration

	ADC1->CR &= ~(ADC_CR_ADEN);		//Disable ADC1 for calibration
	ADC1->CR |= (ADC_CR_ADCAL);		//Calibrate
	//LCDSetCursorLocation(0, 0);		//Wait for calibration
	//LCDSend2LinesWithDelay(500000, "Start ADCCAL    ", "");
	while((ADC1->CR & ADC_CR_ADCAL) != 0)
	{
	}
	//LCDSetCursorLocation(0, 0);
	//LCDSend2LinesWithDelay(500000, "Start ADCCAL    ", "ADCCAL Success  ");

	//Select Clock for ADC

	//LCDSetCursorLocation(0, 0);
	//LCDSend2LinesWithDelay(500000, "Start CLOCK     ", "");

	RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV6;	// Configure the ADC clock
	RCC->AHBENR |= RCC_AHBENR_ADC12EN;	// Enable ADC1 clock
	ADC1_2_COMMON->CCR |=  ADC12_CCR_CKMODE;			//Connect clock to ADC1

	//LCDSetCursorLocation(0, 0);
	//LCDSend2LinesWithDelay(500000, "Start CLOCK     ", "CLOCK Ready     ");


	//ADC Enable

	//LCDSend2LinesWithDelay(500000, "Enable ADC      ", "");
	ADC1->CR |= (ADC_CR_ADEN);					//Enable ADC1
	while((ADC1->ISR & ADC_ISR_ADRD) == 0)
	{
	}
	//LCDSend2LinesWithDelay(500000, "Enable ADC      ", "ADC Enabled     ");
	ADC1->SMPR1 |= ADC_SMPR1_SMP1;				//Set sample rate


	ADC1->CFGR |= ADC_CFGR_CONT; // ADC ContinuousConvMode Enable

	ADC1->SQR1 |= ADC_SQR1_SQ1_4;			//IMP: STARTS CONVERSION ON CHNL 16
	ADC1->SQR1 &= ~ADC_SQR1_L;				//needed?
	ADC1_2_COMMON->CCR |= ADC12_CCR_TSEN;				//enable the sensor
	ADC1_2_COMMON->CCR |= ADC12_CCR_VREFEN;				//enable inbuilt stable vref

	LCDSetCursorLocation(0, 0);
	LCDSend2LinesWithDelay(500000, "TEMP:           ", "               ");

	//float slope = ((float)(110 - 30) / (float)(*TEMP30_CAL_ADDR - *TEMP110_CAL_ADDR));
	//LCDSetCursorLocation(70, 1);
	//LCDSendFloat(slope, 9);
  while (1)
  {
	 ADC1->CR |= ADC_CR_ADSTART;			 //Start conversion
	 while((ADC1->ISR & ADC_ISR_EOC) == 0)	//wait until end of conversion
	 {
	 }
	 //LCDSetCursorLocation(64, 1);	//second line starts from 64
	 temp = (float)(((V25 * 1000.0 - (float)ADC1->DR * 0.8) / AVGSLOPE) + 25.0);
	 LCDSetCursorLocation(5, 0);
	 LCDSendFloat(temp, 4);


  }

}
