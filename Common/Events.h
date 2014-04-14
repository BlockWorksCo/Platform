


#ifndef __EVENTS_H__
#define __EVENTS_H__


#include "Callable.h"
#include "Queue.h"



//
// Handler types...
// NOTE: Cane extend these to pareameterised Handlers.
//


class Handler : public Callable
{
public:
    void operator()()
    {
    }
};


class FunctionHandler : public Handler
{
public:
    FunctionHandler(void (*_pFunction)()) :
        pFunction(_pFunction)
    {
    }

    void operator()()
    {
        pFunction();
    }

    void (*pFunction)();
};


template <typename T>
class MethodHandler : public Handler
{
public:
    MethodHandler(T* _pObject, void (T::*_pMethod)()) :
            pObject(_pObject),
            pMethod(_pMethod)
    {
    }

    void operator()()
    {
        (*pObject.*pMethod)();
    }    

    T*  pObject;
    void (T::*pMethod)();
};


//
//
//
template <  uint16_t maxEventsInQueue,
            uint16_t maxTimedEvents,
            typename TimingType>
class EventEngine : public Callable
{
public:

    //
    // Ctor.
    //
    EventEngine(TimingType& _timing) :
        timing(_timing)
    {
        for(uint16_t i=0; i<maxTimedEvents; i++)
        {
            timedEvents[i].usedFlag = false;
        }
    }


    //
    // Schedule an event handler.
    //
    void Put(Handler& handler, bool& elementDroppedFlag)
    {
        eventQueue.Put(&handler, elementDroppedFlag);
    }


    //
    // Schedule a delayed event handler.
    //
    void PutAfter(uint32_t numberOfTicks, Handler& handler, bool& elementDroppedFlag)
    {
        elementDroppedFlag  = true;

        if(numberOfTicks > 0)
        {
            for(uint16_t i=0; i<maxTimedEvents; i++)
            {
                if(timedEvents[i].usedFlag == false)
                {
                    timedEvents[i].usedFlag                 = true;
                    timedEvents[i].handler                  = &handler;
                    timedEvents[i].reloadValue              = (uint32_t)-1;
                    timedEvents[i].firingTimestamp          = timing.GetMicrosecondTick() + numberOfTicks;

                    elementDroppedFlag  = false;
                    break;
                }
            }
        }
        else
        {
            Put(handler, elementDroppedFlag);
        }
    }


    //
    // Schedule a periodic event handler.
    //
    void PutEvery(uint32_t numberOfTicks, Handler& handler, bool& elementDroppedFlag)
    {
        elementDroppedFlag  = true;

        if(numberOfTicks > 0)
        {
            for(uint16_t i=0; i<maxTimedEvents; i++)
            {
                if(timedEvents[i].usedFlag == false)
                {
                    timedEvents[i].usedFlag                 = true;
                    timedEvents[i].handler                  = &handler;
                    timedEvents[i].reloadValue              = numberOfTicks;
                    timedEvents[i].firingTimestamp          = timing.GetMicrosecondTick() + numberOfTicks;

                    elementDroppedFlag  = false;
                    break;
                }
            }
        }
        else
        {
            Put(handler, elementDroppedFlag);
        }
    }


    //
    // Run the handlers for all immediately available events.
    // Do not block.
    //
    void operator()()
    {
        bool        dataAvailableFlag   = false;        
        Handler*    eventHandler        = eventQueue.Get(dataAvailableFlag);
        uint32_t    currentTime         = timing.GetMicrosecondTick();


        //
        // Run the immediately scheduled event handlers.
        //
        if(dataAvailableFlag == true)
        {
            //
            // Call the handler.
            //
            (*eventHandler)();
        }           

        //
        // Run the timed events handlers.
        //
        for(uint16_t i=0; i<maxTimedEvents; i++)
        {
            //
            // TODO: Handle the past-timestamp case correctly with wraparound...
            //
            if(timedEvents[i].usedFlag == true)
            {
                if(currentTime == timedEvents[i].firingTimestamp)
                {
                    //
                    // Call the event handler.
                    //
                    (*timedEvents[i].handler)();

                    //
                    // Reload or clear the timed event.
                    //
                    if(timedEvents[i].reloadValue != (uint32_t)-1)
                    {
                        timedEvents[i].firingTimestamp  = currentTime + timedEvents[i].reloadValue;
                    }
                    else
                    {
                        timedEvents[i].usedFlag = false;
                    }
                }
            }
        }



    }

private:

    typedef struct    
    {
        Handler*    handler;
        bool        usedFlag;
        uint32_t    reloadValue;
        uint32_t    firingTimestamp;

    } TimedEventHandler;

    TimedEventHandler                               timedEvents[maxTimedEvents];
    Queue<Handler*, maxEventsInQueue, uint8_t>      eventQueue;    
    TimingType&                                     timing;
};






#endif



