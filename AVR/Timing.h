

#ifndef __TIMING_H__
#define __TIMING_H__




//
//
//
template <	uint8_t PortBase,
			typename TimestampType>
class Timing
{

public:

	//
	//
	//
	Timing()
	{
	}


	//
	// Handle the timer IRQ.
	//
	void TickISR()
	{ 
	    //if (TIM_GetITStatus(TIMx, TIM_IT_Update) != RESET)
	    {
	    	//
	    	// Just increment the millisecond counter.
	    	//
	    	microsecondCounter	+= 10;

	        //TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	    }
	}


	//
	// Return the current millisecond tick counter value.
	//
	TimestampType GetMicrosecondTick()
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


	TimestampType	microsecondCounter;
};








#endif


