//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




#ifndef __CYCLECOUNTERTIMING_H__
#define __CYCLECOUNTERTIMING_H__



#include <stm32f4xx_tim.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>


//
//
//
template < typename TimestampType>
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
        TimestampType   partialPeriod   = microsecondCounter;
        do
        {
        } while(partialPeriod == microsecondCounter);

        //
        // We are now at the beginning of a period...
        // Now wait for the specified number of milliseconds.
        //
        TimestampType   endPeriod       = microsecondCounter+(numberOfMilliseconds*1000);
        do
        {
        } while(microsecondCounter != endPeriod);
    }

private:

    //
    //
    //
    uint32_t CycleCount()
    {
        return *DWT_CYCCNT;
    }


    //
    //
    //
    void initialise()
    {
        //
        // - Enable TRCENA bit in DEMCR (0xE000EDFC, bit 24) before using DWT        
        // - You should use DWT's Cycle Count Register (0xE0001004) for timing measurement.
        // The CPI counter is only 8-bit, and need to have trace support to detect the number of time it overflow.
        // While the DWT Cycle Count Register is 32-bit.        
        // - You need to enable the Cycle Count counter.
        // 
        *SCB_DEMCR      |= 0x01000000;
        *DWT_CYCCNT     = 0;            // reset the counter
        *DWT_CONTROL    |= 0x00000001;  // enable the counter
    }



    //
    //
    //
    volatile uint32_t*      DWT_CYCCNT  = (uint32_t *)0xE0001004;
    volatile uint32_t*      DWT_CONTROL = (uint32_t *)0xE0001000;
    volatile uint32_t*      SCB_DEMCR   = (uint32_t *)0xE000EDFC;

    TimestampType   microsecondCounter;
};







#endif






