


#ifndef __AVRGPIO_H__
#define __AVRGPIO_H__





#include "Platform.h" 


//
//
//
template <uint8_t portAddress, uint16_t bitNumber>
class AVROutput
{
public:

    AVROutput()
    {
        pinMode(bitNumber, OUTPUT);
    }

    
    void Set()
    {
        *((volatile uint8_t*)(portAddress+0x20)) |= (1<<bitNumber);
    }
    

    void Clear()
    {
        *((volatile uint8_t*)(portAddress+0x20)) &= ~(1<<bitNumber);
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






