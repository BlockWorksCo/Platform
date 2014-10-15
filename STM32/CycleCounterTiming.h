//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




#ifndef __CYCLECOUNTERTIMING_H__
#define __CYCLECOUNTERTIMING_H__

#include "stm32f4xx.h"
#include "core_cm4.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

//
//
//
template < typename TimestampType, uint32_t ClockRateInHz>
class Timing
{

public:

    //
    //
    //
    Timing() :
        DWTx(*(DWT_Type*)DWT_BASE),
        CoreDebugx(*(CoreDebug_Type*)CoreDebug_BASE)
    {
        //
        // - Enable TRCENA bit in DEMCR (0xE000EDFC, bit 24) before using DWT        
        // - You should use DWT's Cycle Count Register (0xE0001004) for timing measurement.
        // The CPI counter is only 8-bit, and need to have trace support to detect the number of time it overflow.
        // While the DWT Cycle Count Register is 32-bit.        
        // - You need to enable the Cycle Count counter.
        // 
        CoreDebugx.DEMCR |= 0x01000000;
        DWTx.CYCCNT      = 0;            // reset the counter
        DWTx.CTRL        |= 0x00000001;  // enable the counter
    }


    //
    // Return the current millisecond tick counter value.
    //
    TimestampType GetMicrosecondTick()
    {
        return MicrosecondCounter();
    }


    //
    // Busy wait for the specified number of milliseconds.
    //
    void BusyWait(uint32_t numberOfMilliseconds)
    {
        //
        // Wait until we have a clean millisecond boundary...
        //
        TimestampType   partialPeriod   = MicrosecondCounter();
        do
        {
        } while(partialPeriod == MicrosecondCounter());

        //
        // We are now at the beginning of a period...
        // Now wait for the specified number of milliseconds.
        //
        TimestampType   endPeriod       = MicrosecondCounter()+(numberOfMilliseconds*1000);
        do
        {
        } while(MicrosecondCounter() != endPeriod);
    }

private:

    //
    //
    //
    uint32_t CycleCount()
    {
        return DWT->CYCCNT;
    }


    //
    //
    //
    uint32_t MicrosecondCounter()
    {
        return CycleCount()/(ClockRateInHz/1000000);
    }

    //
    //
    //
    uint32_t MillisecondCounter()
    {
        return CycleCount()/(ClockRateInHz/1000);
    }



    //
    //
    //
    DWT_Type&               DWTx;
    CoreDebug_Type&         CoreDebugx;
};







#endif






