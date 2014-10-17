//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




#ifndef __DMAENGINE_H__
#define __DMAENGINE_H__


//
//
//
template <uint32_t baseAddress>
class DMAEngine
{
public:

    //
    //
    //
    DMAEngine()
    {        
    }


    //
    //
    //
    void AsyncTransfer(uint32_t sourceAddress, uint32_t destinationAddress, uint32_t numberOfBytes)
    {

    }

private:


    //
    // Sets up the DMA transfer to the output port.
    //
    void SetupTransfer(uint32_t sourceAddress, uint32_t destinationAddress, uint32_t numberOfBytes)
    {
        DMA_InitTypeDef  DMA_InitStructure;

        /* Enable the DMA clock */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

        /* Configure the DMA Stream */
        DMA_Cmd(DMA2_Stream1, DISABLE);
        DMA_DeInit(DMA2_Stream1);

        /* Set the parameters to be configured */
        DMA_InitStructure.DMA_Channel               = DMA_Channel_7;
        DMA_InitStructure.DMA_Memory0BaseAddr       = sourceAddress; /* Read the data from the RAM buffer */
        DMA_InitStructure.DMA_PeripheralBaseAddr    = destinationAddress;   /* Send GPIO output data register */
        DMA_InitStructure.DMA_DIR                   = DMA_DIR_MemoryToPeripheral;
        DMA_InitStructure.DMA_BufferSize            = numberOfBytes;
        DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;

        DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
        DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Enable;
        DMA_InitStructure.DMA_FIFOThreshold         = DMA_FIFOThreshold_HalfFull;
        DMA_InitStructure.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
        DMA_InitStructure.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
        DMA_Init(DMA2_Stream1, &DMA_InitStructure);

        /* Enable DMA Transfer Complete interrupt */
        DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
    }


    //
    // Configure Timer1 in such a way that it can initiate data transfer using 
    // DMA on rising edge of clock signal received on port pin.
    // 
    void SetupTimer(void)
    {
        const uint32_t          period                  = 10;
        const uint32_t          prescaler               = 10;
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  /* TIM8 clock enable */

        TIM_TimeBaseStructure.TIM_Period        = period;
        TIM_TimeBaseStructure.TIM_Prescaler     = prescaler;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

        TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);  /* Time base configuration */
        TIM_PrescalerConfig(TIM8, ((SystemCoreClock/1200) - 1), TIM_PSCReloadMode_Immediate);
        TIM_ARRPreloadConfig(TIM8, ENABLE);              /* Enable the time autoreload register */
        TIM_Cmd(TIM8, ENABLE);                           /* TIM enable counter */
        TIM_DMACmd(TIM8, TIM_DMA_Update, ENABLE);        /* Enable TIM8_UP DMA Requests */
    }


    //
    //       
    //                     
    void SetupInterrupt(void)
    {
        NVIC_InitTypeDef NVIC_InitStructure;

        /* Enable the DMA Stream IRQ Channel */
        NVIC_InitStructure.NVIC_IRQChannel                      = DMA2_Stream1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }


};

#endif


