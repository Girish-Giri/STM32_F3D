/*
 * DELAY.h
 *
 *  Created on: 02-Nov-2020
 *      Author: GIRISH
 */

#ifndef DELAY_H_
#define DELAY_H_

int myTicks = 0;	//basically microseconds counter variable


//microscnd delay functn:
void dUs(int uS)
{
	TIM4->CR1 |= TIM_CR1_CEN;	//Enable TIM4
	myTicks = 0;				//initiate counter frm 0
	while (myTicks < uS);			//stay until limit
	TIM4->CR1 &= ~(TIM_CR1_CEN);//reset clk to save pwr
}


//milliscnd delay functn:
void dMs(int mS)
{
	dUs(mS * 100); //shortcut, here needed to * by 100 for conversion, technically nt ri8
	/*TIM4->CR1 |= TIM_CR1_CEN;	//Enable TIM4
	myTicks = 0;				//initiate counter frm 0
	while (myTicks < (mS * 1000));	//stay until limit, 1ms = 1000us
	TIM4->CR1 &= ~(TIM_CR1_CEN);//reset clk to save pwr*/
}

void TIM4_IRQHandler()
{
	//IMP if flag not cleared the code WILL stuck at this function
	TIM4->SR &= ~ (TIM_SR_UIF);		//reset Update interrupt flag
	myTicks++;
}


void DelaySet()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;		//Turn on GP timer 4 clock
		TIM4->PSC = 0;						//set pre-scalar, to decide no. pulses to be skipped
		// 72000000/72 = 1000000 (periphrl clk / ARR):
		TIM4->ARR = 72;		//set count limit in auto reload register (1Mhz freq req)
		TIM4->CR1 |= TIM_CR1_URS;	//Only cntr ovrflw/undrflw generates an update interrupt
		TIM4->DIER |= TIM_DIER_UIE;	//UIE: Update interrupt enable
		TIM4->EGR |= TIM_EGR_UG;	//UG: Update Generation

		NVIC_EnableIRQ(TIM4_IRQn);	//abort Enable IrQ line for TIM4
}



#endif /* DELAY_H_ */
