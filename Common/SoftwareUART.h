//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//





#ifndef __SOFTWAREUART_H__
#define __SOFTWAREUART_H__


#include "Utilities.h"




template <  typename RxPinType,
            typename TxPinType,
            uint8_t  SamplesPerBit,
            uint8_t  ConsecutiveSamplesForBit >
class SoftwareUART
{
public:

    SoftwareUART( RxPinType& _rxPin, TxPinType& _txPin ) :
        rxPin(_rxPin),
        txPin(_txPin)
    {
        
    }


    //
    // Called at the bit sampling rate, i.e typically 8x or 16x baud rate.
    //
    void Iterate()
    {
        //
        // Get the current rx level.
        //
        bool    rxLevel     = rxPin.Get();

        static bool         previousLevel       = false;
        static uint8_t      sampleCount         = 0;

        //
        // When sn edge is detected, reset the sampleCount.
        //
        if( previousLevel != rxLevel )
        {
            sampleCount = 0;
        }

        //
        // If we are in the middle of the bit period, presume the line has settled
        // and is now stable. Now we take this sample as the bit value.
        //
        if( sampleCount == (SamplesPerBit/2) )
        {
            ProcessBit( rxLevel );
        }

        //
        // Save for next iteration.
        //
        previousLevel   = rxLevel;
        sampleCount     = (sampleCount+1) % SamplesPerBit;
    }


private:

    //
    //
    //
    typedef enum
    {
        Stopped,
        StartBitDetected,
        Bit0Detected,
        Bit1Detected,
        Bit2Detected,
        Bit3Detected,
        Bit4Detected,
        Bit5Detected,
        Bit6Detected,
        Bit7Detected,
        Bit8Detected,
        StopBitDetected,
    } State;


    //
    //
    //
    void ProcessBit(bool value)
    {
        static State    state;
        static uint8_t  currentByte;

        switch(state)
        {
            case Stopped:
                if( value == false )
                {
                    //
                    // Low bit in Stopped state indicates a Start bit.
                    //
                    state           = StartBitDetected;
                    currentByte     = 0;
                }
                break;

            case StartBitDetected:
                currentByte     |= value;           // Bit 0
                state           = Bit0Detected;
                break;

            case Bit0Detected:
                currentByte     |= (value << 1);    // Bit 1
                state           = Bit1Detected;
                break;

            case Bit1Detected:
                currentByte     |= (value << 2);    // Bit 2
                state           = Bit2Detected;
                break;

            case Bit2Detected:
                currentByte     |= (value << 3);    // Bit 3
                state           = Bit3Detected;
                break;

            case Bit3Detected:
                currentByte     |= (value << 4);    // Bit 4
                state           = Bit4Detected;
                break;

            case Bit4Detected:
                currentByte     |= (value << 5);    // Bit 5
                state           = Bit5Detected;
                break;

            case Bit5Detected:
                currentByte     |= (value << 6);    // Bit 6
                state           = Bit6Detected;
                break;

            case Bit6Detected:
                currentByte     |= (value << 7);    // Bit 7
                state           = Bit7Detected;
                break;

            case Bit7Detected:
                if( value == true )
                {
                    //
                    // High bit after last data bit indicates a Stop bit.
                    //
                    state           = Stopped;
                    ProcessByte( currentByte );
                }
                break;

            default:
                state   = Stopped;
                break;
        }
    }


    //
    //
    //
    void ProcessByte(bool value)
    {
        static uint8_t      buffer[64];
        static uint8_t      head        = 0;

        buffer[head]   = value;
        head            = (head+1) % NUMBER_OF_ELEMENTS(buffer);
    }


    //
    //
    //
    RxPinType&  rxPin;
    TxPinType&  txPin;

};


#endif



