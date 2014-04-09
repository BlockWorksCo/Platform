

#ifndef __TIMING_H__
#define __TIMING_H__


#include <stm32f4xx_tim.h>


//
//
//
template <	uint32_t PortBase,
			typename TimestampType>
class Timing
{

public:

	//
	//
	//
	Timing() :
		TIMx((TIM_TypeDef*)PortBase),
		microsecondCounter(0)
	{
		initialise();
		LED_Config();
	}


	//
	// Handle the timer IRQ.
	//
	void ISR()
	{ 
	    if (TIM_GetITStatus(TIMx, TIM_IT_Update) != RESET)
	    {
	    	//
	    	// Just increment the millisecond counter.
	    	//
	    	microsecondCounter	+= 10;

	        TIM_ClearITPendingBit(TIMx, TIM_IT_Update);

	        //
	        // Toggle the LED at 2Hz
	        //
	        //if((millisecondCounter % 500) == 0)
	        {
	        	GPIO_ToggleBits(GPIOD, GPIO_Pin_13);	        	        	
	        }
	    }
	}

	//
	// Return the current millisecond tick counter value.
	//
	TimestampType GetTick()
	{
		return microsecondCounter;
	}

	//
	// Busy wait for the specified number of milliseconds.
	//
	void BusyWait(uint32_t numberOfMilliseconds)
	{
		//
		// Wait until we have a clean millisecond boundary...
		//
		TimestampType	partialPeriod 	= microsecondCounter;
		do
		{
		} while(partialPeriod == microsecondCounter);

		//
		// We are now at the beginning of a period...
		// Now wait for the specified number of milliseconds.
		//
		TimestampType	endPeriod 		= microsecondCounter+(numberOfMilliseconds*1000);
		do
		{
		} while(microsecondCounter != endPeriod);
	}

private:


	//
	//
	//
	void LED_Config(void)
	{
		GPIO_InitTypeDef  	GPIO_InitStructure;

		/* GPIOD Periph clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType 		= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}

	//
	//
	//
	void initialise()
	{
		NVIC_InitTypeDef 		NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

		/* Enable the TIM2 gloabal Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel 						= TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/* TIM2 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period 			= 10 - 1; // 1 MHz down to 1 KHz (1 ms)
		TIM_TimeBaseStructure.TIM_Prescaler 		= 84 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)
		TIM_TimeBaseStructure.TIM_ClockDivision 	= 0;
		TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

		/* TIM IT enable */
		TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);

		/* TIM2 enable counter */
		TIM_Cmd(TIMx, ENABLE);
	}


	TIM_TypeDef*	TIMx;
	TimestampType	microsecondCounter;
};








#endif


