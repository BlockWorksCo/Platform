


#ifndef __WIN32GPIO_H__
#define __WIN32GPIO_H__





#include "Platform.h" 
#include <string.h>



typedef enum
{
    OrangeLED   = 0,
    RedLED      = 1,
    GreenLED    = 2,
    BlueLED     = 3,

} OutputID;




typedef enum
{
    ButtonA     = 0,

} InputID;




//
//
//
template <OutputID ID>
class Win32Output 
{
public:

    Win32Output()
    {
        printf("devID(%d)\n", ID);
    }

    
    void Toggle()
    {
    }
    
    void Set()
    {
    }
    
    void Set(uint16_t value)
    {
    }
    
    void Clear()
    {
    }
    
};



//
//
//
template <  InputID ID, 
            typename valueChangeDelegateType
         >
class Win32Input 
{
public:

    Win32Input(valueChangeDelegateType&  _valueChangeDelegate) :
            valueChangeDelegate(_valueChangeDelegate)
    {
    }

    
    uint16_t Get()
    {
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






