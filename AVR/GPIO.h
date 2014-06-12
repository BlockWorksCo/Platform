


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

        bitNum  = digitalPinToBitMask(port);
        portNum = digitalPinToPort(port); //
        out     = portOutputRegister(portNum);
    }

    
    void Toggle()
    {
    }
    
    void Set()
    {
        *out |= (bitNum);
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
        *out &= ~bitNum;
    }

private:

    uint8_t             bitNum;
    uint8_t             portNum;
    volatile uint8_t*   out;;    
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






