


#ifndef __AVRGPIO_H__
#define __AVRGPIO_H__





#include "Platform.h" 


//
//
//
template <uint16_t portData, uint16_t portDirection, uint8_t mask>
class AVROutput 
{
public:

    AVROutput()
    {
	    *((volatile uint8_t*)portDirection)  |= mask; 		// P1.0 and P1.6 are the red+green LEDs	
    }

    
    void Toggle()
    {
	   *((volatile uint8_t*)portData)    ^= mask; 			// Toggle P1.6 output (green LED) using exclusive-OR
    }
    
    void Set()
    {
	    *((volatile uint8_t*)portData)  &= mask; 		// All LEDs off
    }
    
    void Set(uint16_t value)
    {
	    *((volatile uint8_t*)portData)  = ((*((volatile uint8_t*)portData)) & (~mask)) | value; 		// All LEDs off
    }
    
    void Clear()
    {
	    *((volatile uint8_t*)portData)  &= ~mask; 		// All LEDs off
    }
    
};



//
//
//
template <  uint16_t portData,
            uint16_t portDirection, 
            uint16_t mask,  
            typename valueChangeDelegateType>
class AVRInput 
{
public:

    AVRInput(valueChangeDelegateType&  _valueChangeDelegate) :
            valueChangeDelegate(_valueChangeDelegate)
    {
        *((volatile uint8_t*)portDirection)  &= ~mask; 
    }
    
    uint8_t Get()
    {
        return (*((volatile uint8_t*)portData))&mask;
    }

    void Poll()
    {
        valueChangeDelegate();
    }
    
private:

    valueChangeDelegateType&    valueChangeDelegate;
};





#endif






