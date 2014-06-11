


#ifndef __AVRGPIO_H__
#define __AVRGPIO_H__





#include "Platform.h" 


//
//
//
template <uint16_t port>
class AVROutput 
{
public:

    AVROutput()
    {
        pinMode(port, OUTPUT);
    }

    
    void Toggle()
    {
    }
    
    void Set()
    {
        uint8_t             bitNum  = digitalPinToBitMask(port);
        uint8_t             portNum = digitalPinToPort(port);
        volatile uint8_t*   out     = portOutputRegister(portNum);

        *out |= bitNum;
    }
    
    void Set(uint16_t value)
    {
        if(value == 0)
        {
            Clear();
        }
        else
        {
            Set();
        }
    }
    
    void Clear()
    {
        uint8_t             bitNum  = digitalPinToBitMask(port);
        uint8_t             portNum = digitalPinToPort(port);
        volatile uint8_t*   out     = portOutputRegister(portNum);

        *out &= ~bitNum;
    }
    
};



//
//
//
template <  uint16_t portData,
            typename valueChangeDelegateType>
class AVRInput 
{
public:

    AVRInput(valueChangeDelegateType&  _valueChangeDelegate) :
            valueChangeDelegate(_valueChangeDelegate)
    {
        //*((volatile uint8_t*)portDirection)  &= ~mask; 
    }
    
    uint8_t Get()
    {
        //return (*((volatile uint8_t*)portData))&mask;
        return 0;
    }

    void Poll()
    {
        valueChangeDelegate();
    }
    
private:

    valueChangeDelegateType&    valueChangeDelegate;
};





#endif






