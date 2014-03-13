


#ifndef __LINUXGPIO_H__
#define __LINUXGPIO_H__





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
class LinuxOutput 
{
public:

    LinuxOutput()
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
class LinuxInput 
{
public:

    LinuxInput(valueChangeDelegateType&  _valueChangeDelegate) :
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






