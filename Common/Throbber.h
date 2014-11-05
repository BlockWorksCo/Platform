//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//



#ifndef __THROBBER_H__
#define __THROBBER_H__


//
// NOTE:
// No coupling, i.e. '#include's .... this is by design, lets keep it this way!
// All dependencies are injected via the template parameters.
//




//
// Class to perform a modulated PWM onto a specified GPIO in order to
// perform a Throbber or Breathing type effect.
//
template <  typename gpioType,
            uint32_t maxPeriodInCycles,
            uint32_t minPeriodInCycles,
            uint32_t speed >
class Throbber
{
public:

    Throbber(gpioType& _gpio, int32_t initialValue) :
        gpio(_gpio),
        cycleCount(0),
        currentOnDuration(initialValue),
        state(false),
        onDurationDelta(1)
    {

    }


    void Iterate()
    {
        //
        // First part of period is the 'on' phase, second the 'off' phase.
        //
        if(cycleCount >= currentOnDuration)
        {
            state   = false;
        }
        else
        {
            state   = true;
        }

        //
        // Do the dirty work.
        //
        if(state == false)
        {
            gpio.Set();
        }
        else
        {        
            gpio.Clear();
        }

        //
        // Keep track of the position in the period.
        //
        cycleCount++;
        if(cycleCount >= maxPeriodInCycles)
        {
            cycleCount  = 0;        

            //
            //
            //
            currentOnDuration   += onDurationDelta;
            if(currentOnDuration > maxPeriodInCycles)
            {
                onDurationDelta     = -speed;
                currentOnDuration   = maxPeriodInCycles;
            }
            else if(currentOnDuration < minPeriodInCycles)
            {
                onDurationDelta     = speed;
                currentOnDuration   = minPeriodInCycles;
            }
        }


    }

private:

    gpioType&   gpio;
    int32_t     cycleCount;
    int32_t     currentOnDuration;
    bool        state;
    int32_t     onDurationDelta;
};


#endif


#if 0




//
// LED GPIO definitions.
//
typedef STM32Output<GPIOD_BASE, 12>     GreenLEDType;
typedef STM32Output<GPIOD_BASE, 13>     OrangeLEDType;
typedef STM32Output<GPIOD_BASE, 14>     RedLEDType;
typedef STM32Output<GPIOD_BASE, 15>     BlueLEDType;

GreenLEDType        greenLED;
OrangeLEDType       orangeLED;
RedLEDType          redLED;
BlueLEDType         blueLED;


typedef Throbber<GreenLEDType, 100, 10, 2>  GreenThrobberType;
GreenThrobberType                           greenThrobber(greenLED, 5);
MethodHandler<GreenThrobberType>            greenThrobberEvent( greenThrobber, &GreenThrobberType::Iterate );

typedef Throbber<OrangeLEDType, 100, 10, 2> OrangeThrobberType;
OrangeThrobberType                          orangeThrobber(orangeLED, 33);
MethodHandler<OrangeThrobberType>           orangeThrobberEvent( orangeThrobber, &OrangeThrobberType::Iterate );

typedef Throbber<RedLEDType, 100, 10, 2>    RedThrobberType;
RedThrobberType                             redThrobber(redLED, 66);  // Ooo-err missuss...
MethodHandler<RedThrobberType>              redThrobberEvent( redThrobber, &RedThrobberType::Iterate );

typedef Throbber<BlueLEDType, 100, 10, 2>   BlueThrobberType;
BlueThrobberType                            blueThrobber(blueLED, 95);
MethodHandler<BlueThrobberType>             blueThrobberEvent( blueThrobber, &BlueThrobberType::Iterate );



#endif


