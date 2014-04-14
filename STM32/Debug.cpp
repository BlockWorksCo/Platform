

#include <stdint.h>
#include "stm32f4xx_gpio.h"




void SetupDebugPin()
{
    GPIO_InitTypeDef    GPIO_InitStructure;

    //
    // Debug pin.
    //
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_11|GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType       = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd        = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void SetDebugPin()
{
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
}

void ClearDebugPin()
{
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);     
}

void SetDebugPin2()
{
    GPIO_SetBits(GPIOD, GPIO_Pin_11);
}

void ClearDebugPin2()
{
    GPIO_ResetBits(GPIOD, GPIO_Pin_11);     
}




