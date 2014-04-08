


#ifndef __EVENTS_H__
#define __EVENTS_H__


#include "Callable.h"
#include "Queue.h"




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
template <uint16_t maxEventsInQueue>
class EventEngine : public Callable
{
public:

    //
    //
    //
    void Put(Handler& handler, bool& elementDroppedFlag)
    {
        eventQueue.Put(&handler, elementDroppedFlag);
    }


    //
    // Run the handlers for all immediately available events.
    // Do not block.
    //
    void operator()()
    {
        bool     dataAvailableFlag   = false;        
        Handler*  eventHandler        = eventQueue.Get(dataAvailableFlag);

        if(dataAvailableFlag == true)
        {
            //
            // Call the handler.
            //
            (*eventHandler)();
        }           

    }

private:

    Queue<Handler*, maxEventsInQueue, uint8_t>     eventQueue;
};






#endif



