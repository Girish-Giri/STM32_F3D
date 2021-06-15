#include "stm32f3xx.h"
#include "LCD.h"
#include "DELAY.h"
#include "ADC.h"


void configurepin(void){
	RCC-> AHBENR |= (1<<17) | (1<<22);	//io-port A & F RCC enable
	//here ADC channels 5 is used.
	//set GPIO moder register to ANALOG MODE (GPIO F4 has ADC1_IN5):
	GPIOF-> MODER |=  (GPIO_MODER_MODER4_0) | (GPIO_MODER_MODER4_1);//set moder to make 00 to 11
}

void ADC1_2_IRQHandler() //existing macro functn
{
	 LCDSetCursorLocation(0, 0);
	 LCDSend2LinesWithDelay(0, "POT:", "");
	 LCDSetCursorLocation(5, 0);
	 LCDSendInteger(ADC1->DR, 5);
}


int main(void)
{
	LCDInitializePort();
	LCDSetup();
	configurepin();
	ADCInitialize();
	ADCSetInterruptMode();
	ADCChannel(5);  						//IMP: STARTS CONVERSION ON CHNL 5
	ADCRun();								//Start ADC1
  while (1)
  {

  }
}
