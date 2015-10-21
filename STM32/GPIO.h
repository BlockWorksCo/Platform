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
#include "stm32f4xx.h"

#define ODR_OFFSET                          (offsetof( GPIO_TypeDef, ODR))
#define IDR_OFFSET                          (offsetof( GPIO_TypeDef, IDR))

#define BITBAND_PERIPH_BASE  0x42000000
#define SET_BIT_PERIPH(addx, bitN, value)   *( (volatile unsigned long*) ((BITBAND_PERIPH_BASE + ((addx)-PERIPH_BASE)*32 + (bitN*4))) ) = value
#define GET_BIT_PERIPH(addx, bitN)          *( (volatile unsigned long*) ((BITBAND_PERIPH_BASE + ((addx)-PERIPH_BASE)*32 + (bitN*4))) )
#define TOGGLE_BIT_PERIPH(addx, bitN)       *( (volatile unsigned long*) ((BITBAND_PERIPH_BASE + ((addx)-PERIPH_BASE)*32 + (bitN*4))) ) = !(*( (volatile unsigned long*) ((BITBAND_PERIPH_BASE + ((addx)-PERIPH_BASE)*32 + (bitN*4))) ))



//
//
//
template <uint32_t portAddress, uint8_t bitNumber>
class STM32Output 
{
public:

    STM32Output() :
        mask(1<<bitNumber)
    {
        GPIO_InitTypeDef    GPIO_InitStructure;

        if(portAddress == GPIOA_BASE)
        {
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
        }
        if(portAddress == GPIOB_BASE)
        {
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
        }
        if(portAddress == GPIOC_BASE)
        {
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
        }
        if(portAddress == GPIOD_BASE)
        {
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
        }
        if(portAddress == GPIOE_BASE)
        {
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
        }
        if(portAddress == GPIOF_BASE)
        {
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
        }
        if(portAddress == GPIOG_BASE)
        {
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
        }
        if(portAddress == GPIOH_BASE)
        {
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
        }

        //
        // Debug pin.
        //
        GPIO_InitStructure.GPIO_Pin        = mask;
        GPIO_InitStructure.GPIO_Mode       = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType      = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed      = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd       = GPIO_PuPd_UP;
        GPIO_Init((GPIO_TypeDef*)portAddress, &GPIO_InitStructure);    
    }

    
    void Toggle()
    {
        //((GPIO_TypeDef*)portAddress)->ODR   ^= mask;
        TOGGLE_BIT_PERIPH( portAddress+ODR_OFFSET, bitNumber );
    }
    
    void Set()
    {
        //((GPIO_TypeDef*)portAddress)->ODR   |= mask;
        SET_BIT_PERIPH(portAddress+ODR_OFFSET, bitNumber, 1);
    }
    
    void Set(uint16_t value)
    {
        //((GPIO_TypeDef*)portAddress)->ODR   = (((GPIO_TypeDef*)portAddress)->ODR & ~mask) | (value<<bitNumber);
        SET_BIT_PERIPH(portAddress+ODR_OFFSET, bitNumber, value);
    }
    
    void Clear()
    {
        //((GPIO_TypeDef*)portAddress)->ODR   &= ~mask;
        SET_BIT_PERIPH(portAddress+ODR_OFFSET, bitNumber, 0);
    }
    

private:

    uint16_t    mask;
};



//
//
//


//
//
//
template <  uint32_t portAddress, 
            uint8_t bitNumber >
class STM32Input 
{
public:

    STM32Input()
    {
        GPIO_InitTypeDef    GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Pin       = 1<<bitNumber;
        GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_OType     = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd      = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed     = GPIO_Speed_50MHz;
        GPIO_Init((GPIO_TypeDef*)portAddress, &GPIO_InitStructure);    
    }
    

    bool Get()
    {
        return GET_BIT_PERIPH( portAddress+IDR_OFFSET, bitNumber );
    }

};




#endif






