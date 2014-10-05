//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




#ifndef __STM32GPIO_H__
#define __STM32GPIO_H__





#include "Platform.h" 
#include "stm32f4xx_gpio.h"


//
//
//
template <uint32_t portAddress, uint16_t mask>
class STM32Output 
{
public:

    STM32Output()
    {
        GPIO_InitTypeDef    GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Pin       = mask;
        GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType     = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd      = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed     = GPIO_Speed_100MHz;
        GPIO_Init((GPIO_TypeDef*)portAddress, &GPIO_InitStructure);    
        
        ((GPIO_TypeDef*)portAddress)->ODR   |= mask;
        
        //if(portAddress == GPIOD_BASE)
        {
	        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
        }
    }

    
    void Toggle()
    {
        ((GPIO_TypeDef*)portAddress)->ODR   ^= mask;
    }
    
    void Set()
    {
        ((GPIO_TypeDef*)portAddress)->ODR   |= mask;
    }
    
    void Set(uint16_t value)
    {
        ((GPIO_TypeDef*)portAddress)->ODR   |= value;
    }
    
    void Clear()
    {
        ((GPIO_TypeDef*)portAddress)->ODR   &= ~mask;
    }
    
};



//
//
//


//
//
//
template <  uint32_t portAddress, 
            uint16_t mask,  
            typename valueChangeDelegateType>
class STM32Input 
{
public:

    STM32Input(valueChangeDelegateType&  _valueChangeDelegate) :
            valueChangeDelegate(_valueChangeDelegate)
    {
        GPIO_InitTypeDef    GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Pin       = mask;
        GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_OType     = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd      = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed     = GPIO_Speed_50MHz;
        GPIO_Init((GPIO_TypeDef*)portAddress, &GPIO_InitStructure);    
        
        //((GPIO_TypeDef*)portAddress)->ODR   &= ~mask;
    }
    
    bool Get()
    {
        return (((GPIO_TypeDef*)portAddress)->IDR & mask) == 0;
    }

    void Poll()
    {
        valueChangeDelegate();
    }
    
private:

    valueChangeDelegateType&    valueChangeDelegate;
};




#endif






