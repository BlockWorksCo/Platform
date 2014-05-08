



#ifndef __SPI_H__
#define __SPI_H__


#include <stm32f4xx_conf.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx.h>
#include <misc.h>
#include <stm32f4xx_spi.h>
#include "Events.h"
#include "Debug.h"





template <uint32_t PortBase>
class SPI
{

public:

    //
    //
    //
    SPI() :
        SPIx((SPI_TypeDef*)PortBase)
    {
        initialise();
    }


    //
    // As master, clock a byte out of MOSI.
    //
    void Put(uint8_t data)
    {         
        while(!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)); 
        SPI_I2S_SendData(SPIx, data);
        while(!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE));
        SPI_I2S_ReceiveData(SPIx);
    }


    //
    // As a master, clock a byte into MISO.
    //
    uint8_t Get()
    {         
        while(!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)); 
        SPI_I2S_SendData(SPIx, 0x00);   // Dummy byte to generate clock
        while(!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE));
         
        return SPI_I2S_ReceiveData(SPIx);
    }


    //
    //
    //
    void ISR()
    {
        if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_TXE) == SET)
        {
            static uint16_t     count   = 0;

            SPIx->DR  = count;
            count++;
            //SPI_Cmd(SPIx, DISABLE);

        }

        if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_RXNE) == SET)
        {
            static uint16_t     i       = 0 ;

            buffer[i] = SPIx->DR ;
            i   = (i+1) % sizeof(buffer);

        }
    }

private:

    //
    //
    //
    void initialise(void)
    {
        GPIO_InitTypeDef    GPIO_InitStructure;
        SPI_InitTypeDef     SPI_InitStruct;

        // enable peripheral clock
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

        // enable clock for used IO pins
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

        /* configure pins used by SPI3
        * PC10 = SCK
        * PC11 = MISO
        * PC12 = MOSI
        */
        GPIO_InitStructure.GPIO_Pin             = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode            = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType           = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed           = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_PuPd            = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        // connect SPI3 pins to SPI alternate function
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

        GPIO_PinAFConfig(GPIOA, GPIO_PinSource15,GPIO_AF_SPI3);
        GPIO_InitStructure.GPIO_Pin             = GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Speed           = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode            = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType           = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd            = GPIO_PuPd_UP;
        GPIO_Init( GPIOA, &GPIO_InitStructure );


        SPI_InitStruct.SPI_Direction            = SPI_Direction_2Lines_FullDuplex;  // set to full duplex
        SPI_InitStruct.SPI_Mode                 = SPI_Mode_Slave;                   // transmit in slave mode, NSS pin has
        SPI_InitStruct.SPI_DataSize             = SPI_DataSize_8b;                  // one packet of data is 8 bits wide
        SPI_InitStruct.SPI_CPOL                 = SPI_CPOL_Low;                     // clock is low when idle
        SPI_InitStruct.SPI_CPHA                 = SPI_CPHA_1Edge;                   // data sampled at first edge
        SPI_InitStruct.SPI_NSS                  = SPI_NSS_Hard;                     // set the NSS HARD
        SPI_InitStruct.SPI_BaudRatePrescaler    = SPI_BaudRatePrescaler_256;        // SPI frequency is
        SPI_InitStruct.SPI_FirstBit             = SPI_FirstBit_MSB;                 // data is transmitted MSB first
        SPI_Init(SPIx, &SPI_InitStruct);

        SPI_Cmd(SPIx, ENABLE); // enable SPI3    
    }


    SPI_TypeDef*    SPIx;
    uint8_t         buffer[10];

};







#endif


