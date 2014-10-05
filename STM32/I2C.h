//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//



#ifndef __I2C_H__
#define __I2C_H__



#include <stm32f4xx_conf.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx.h>
#include <misc.h>
#include <stm32f4xx_i2c.h>
#include "Events.h"
#include "Debug.h"


//
// Enumeration of the possible command types.
//
typedef enum 
{
	i2cMasterWrite 	= 0,
	i2cMasterRead  	= 1,
	i2cError 	  	= 2,
	i2cSlaveWrite 	= 3,
	i2cSlaveRead  	= 4,
	i2cGeneralCall 	= 5,

} I2CCommandType;


const uint32_t 	maxPayloadSize 	= 16;

//
//
//
typedef struct
{
	I2CCommandType	type;
	uint16_t		target;	
	uint8_t			numberOfBytes;
	uint8_t			payload[maxPayloadSize];

} I2CCommand;



//
//
//
typedef enum 
{
	i2cIdle 	= 0,
	i2cStart 	= 1,
	i2cStop 	= 2,
	i2cReadACK 	= 3,
	i2cReadNACK	= 4,
	i2cWrite	= 5,

} I2CState;


//
//
//
template <	uint32_t PortBase,
			typename TxQueueType, 
			typename RxQueueType,
			uint32_t speed,
			uint16_t ownAddress,
			typename EventEngineType>
class I2C
{
public:

	//
	// Initialise the queue.
	//
	I2C(TxQueueType& _txQueue, RxQueueType& _rxQueue, EventEngineType& _eventEngine) :
		txQueue(_txQueue),
		rxQueue(_rxQueue),
		I2Cx((I2C_TypeDef*)PortBase),
		queueTooSmallFlag(false),
		state(i2cIdle),
		subState(0),
		slaveResponseData(0),
		numberOfResponseBytes(0),
		errorFlags(0),
		eventEngine(_eventEngine),
		respondToSlaveReadHandler(this, &I2C::RespondToSlaveRead),
		respondToSlaveWriteHandler(this, &I2C::RespondToSlaveWrite),
		addressMatchedHandler(this, &I2C::AddressMatched),
		slaveModeSTOPHandler(this, &I2C::slaveModeSTOP),
		slaveReadDelay(0),
		eventQTooSmallFlag(false)		
	{

		currentCommand.numberOfBytes 	= 0;
		slaveMode				= false;
		busBusy					= false;
		receiveMode				= false;
		byteTransferFinished	= false;

		initialise();
	}


	//
	//
	//
	void SetSlaveResponse(uint8_t* response, uint16_t _numberOfResponseBytes)
	{
		slaveResponseData 		= response;
		numberOfResponseBytes	= _numberOfResponseBytes;
	}

	//
	//
	//
	void SetSlaveReadDelay(uint32_t numberOfMicroseconds)
	{
	    //SetDebugPin2();
		slaveReadDelay 	= numberOfMicroseconds;
	    //ClearDebugPin2();
	}

	//
	//
	//
	bool IsBusy()
	{
		bool 	returnValue 	= false;

		if( I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != 0 )
		{
			returnValue 	= true;
		}

		return returnValue;
	}


	//
	//
	//
	bool IsIdle()
	{
		return (state == i2cIdle);
	}


	//
	//
	//
	void BusyWaitTillIdle()
	{
		while(state != i2cIdle);
	}


	//
	// Process the master command queue.
	//
	bool ProcessQueue()
	{
		bool		dataAvailableFlag	= false;
		I2CCommand 	command				= txQueue.Get(dataAvailableFlag);

		if(dataAvailableFlag == true)
		{
			switch(command.type)
			{
				default:
				{
					break;
				}
			}
		}

		return dataAvailableFlag;
	}




	//
	//
	//
	void StateMachine(I2CState&	state,	uint8_t& subState)
	{
		static uint8_t	counter 				= 0;
		uint16_t 		sr1						= I2Cx->SR1;
		uint16_t 		sr2						= I2Cx->SR2;

		//
		// Determine the state of the common flags.
		//
		slaveMode				= false;
		busBusy					= false;
		receiveMode				= false;
		byteTransferFinished	= false;

		if( (sr2&0x0001) == 0)		// MSL bit.
		{
			slaveMode				= true;
		}

		if( (sr2&0x0002) != 0)		// BUSY bit.
		{
			busBusy					= true;
		}

		if( (sr2&0x0004) == 0)		// TRA bit.
		{
			receiveMode				= true;
		}

		if( (sr1&0x0004) != 0)		// BTF bit.
		{
			byteTransferFinished	= true;
		}

		//
		// NOTE: Ordering of event handlers & regeister accesses *IS* important here.
		//
		switch(state)
		{

			case i2cIdle:
			{
				if( (sr1&0x0002) != 0)		// ADDR bit
				{
					//
					// Slave mode address acknowledged.
			        // The slave stretches SCL low until ADDR is
			        // cleared and DR filled with the data to be sent
			        // *Note: Cleared by read of SR1/2 above.
					//
					eventEngine.Put(addressMatchedHandler, eventQTooSmallFlag);
				}

				if( (sr1&0x0040) != 0)		// RxNE bit
				{
					eventEngine.Put(respondToSlaveWriteHandler, eventQTooSmallFlag);
				}

				if( (sr1&0x0010) != 0)		// STOPF bit
				{
					//
					// Clear the STOPF interrupt by a read from SR1 plus write to CR1 clears the STOPF interrupt pending.
					//
					I2Cx->SR1 &= (~0x10);
					I2Cx->CR1 |= 0x1;

					eventEngine.Put(slaveModeSTOPHandler, eventQTooSmallFlag);
				}
				
				if( (sr1&0x0080) != 0)		// TxE bit
				{
					//
					// Clear the TxE interrupt pending bit and schedule a handler for this event.
					//
					I2Cx->SR1 &= (~0x80);

					//
					// If BTF != 0 then the shift register is empty and we can fill DR again....
					// Byte transfer finished...
					//
					eventEngine.PutAfter(slaveReadDelay, respondToSlaveReadHandler, eventQTooSmallFlag);
					I2C_ITConfig(I2Cx, I2C_IT_EVT, DISABLE); 	// disable the I2Cx event interrupt 
				}

			}


			default:
			{
				//STOP;
				break;
			}
		}

	}


	//
	// Called from interrupt level to service the I2C event IRQ.
	//
	void EventISR(void)
	{
		StateMachine(state,	subState);
	}

	//
	// Called from interrupt level to service the I2C error IRQ.
	// Errors can be:
	// - Buss Error
	// - Arbitration Loss.
	// - Ack Failure.
	// - Over/Under run.
	//
	void ErrorISR(void)
	{
		//
		// TODO: Report the errors back.
		//
		uint32_t 	SR1Register;
		uint32_t 	SR2Register;

		/* Read the I2C1 status register */
		SR1Register = I2C1->SR1;
		if(SR1Register & 0x0F00) 
		{	//an error
			errorFlags	= ((SR1Register&0x0F00)>>8);//save error
		}

		/* If AF, send STOP*/
		if(SR1Register & 0x0400)
		{
			I2C_GenerateSTOP(I2C1, ENABLE);//program the Stop			

			//
			// Transaction has finished, lets put the command in the queue for the App to process.
			//
			rxQueue.Put(currentCommand, queueTooSmallFlag);			
		}

		/* If BERR, send STOP*/
		if(SR1Register & 0x0100)
		{
			I2C_GenerateSTOP(I2C1,ENABLE);//program the Stop			

			//
			// Transaction has finished, lets put the command in the queue for the App to process.
			//
			currentCommand.type 			= i2cError;
			currentCommand.numberOfBytes 	= 0;
			rxQueue.Put(currentCommand, queueTooSmallFlag);			
		}

		/* If AF, BERR or ARLO, abandon the current job and send a start to commence new */
		if(SR1Register & 0x0700) 
		{
			SR2Register = I2C1->SR2;//read second status register to clear ADDR if it is set (note that BTF will not be set after a NACK)
			I2C_ITConfig(I2C1, I2C_IT_BUF, DISABLE);//disable the RXNE/TXE interrupt - prevent the ISR tailchaining onto the ER (hopefully)
		}

		I2C1->SR1 &=~0x0F00;		//reset all the error bits to clear the interrupt		
	}


	//
	//
	//
	void slaveModeSTOP()
	{
		//
		// Transaction has finished, lets put the command in the queue for the App to process.
		//
		rxQueue.Put(currentCommand, queueTooSmallFlag);
	}

	//
	//
	//
	void RespondToSlaveRead()
	{
		//
		// The transmit buffer is empty, put data into it to send to the master.					
		//
        I2Cx->DR 	= slaveResponseData[currentCommand.numberOfBytes];
		I2C_ITConfig(I2Cx, I2C_IT_EVT, ENABLE); 	// disable the I2Cx event interrupt 

        //
        // Store what we've transmitted into the currentCommand payload so we can notify the host of 
        // what we've transmitted back to the master.
        //
        currentCommand.payload[currentCommand.numberOfBytes] = slaveResponseData[currentCommand.numberOfBytes];
        currentCommand.numberOfBytes 	= (currentCommand.numberOfBytes + 1) % numberOfResponseBytes;		
	}

	//
	//
	//
	void RespondToSlaveWrite()
	{
		I2Cx->CR1 	|= 0x400;
		
		//
		// The data buffer is full. Read the byte from it.
		//
        currentCommand.payload[currentCommand.numberOfBytes] = I2Cx->DR;
        currentCommand.numberOfBytes 	= (currentCommand.numberOfBytes + 1) % maxPayloadSize;
	}


	//
	//
	//
	void AddressMatched()
	{
		I2Cx->SR1 &= (~0x0002);

		if(slaveMode == true)
		{
			currentCommand.numberOfBytes 	= 0;
			currentCommand.target 			= I2Cx->OAR1;		// 7-bit address for now. TODO: support 10-bit.						
		}

		if(receiveMode == true)
		{
			//
			// We're starting a slave-write transaction so we're
			// about to receive data from the master.
			//
			currentCommand.type 			= i2cSlaveWrite;
		}
		else
		{
			//
			// We're starting a slave-read transaction so we're
			// about to transmit data to the master.
			//
			currentCommand.type 			= i2cSlaveRead;
		}
	}

private:	


	/* This funcion initializes the USART1 peripheral
	 * 
	 * Arguments: baudrate --> the baudrate at which the USART is 
	 * 						   supposed to operate
	 */
	void initialise()
	{
		GPIO_InitTypeDef 	GPIO_InitStruct;
		I2C_InitTypeDef 	I2C_InitStruct;
		NVIC_InitTypeDef 	NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)

		//
		// Configure interrupts.
		//
		NVIC_InitStructure.NVIC_IRQChannel 						= I2C1_EV_IRQn;					// we want to configure the I2C1 interrupts
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;							// this sets the priority group of the I2C1 interrupts
		NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;		 					// this sets the subpriority inside the group
		NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			 			// the I2C1 interrupts are globally enabled
		NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff	

		NVIC_InitStructure.NVIC_IRQChannel 						= I2C1_ER_IRQn;					// we want to configure the I2C1 interrupts
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;							// this sets the priority group of the I2C1 interrupts
		NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;		 					// this sets the subpriority inside the group
		NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;			 			// the I2C1 interrupts are globally enabled
		NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff	
		//
		// I2C initialise
		//
		I2C_SoftwareResetCmd(I2Cx, ENABLE);
		I2C_SoftwareResetCmd(I2Cx, DISABLE);

		// enable APB1 peripheral clock for I2C1
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

		// enable clock for SCL and SDA pins
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		//
		// Reset the I2C peripheral.
		//
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
  		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

		/* setup SCL and SDA pins
		 * You can connect I2C1 to two different
		 * pairs of pins:
		 * 1. SCL on PB6 and SDA on PB7 
		 * 2. SCL on PB8 and SDA on PB9 <--
		 */
		GPIO_InitStruct.GPIO_Pin 		= GPIO_Pin_8 | GPIO_Pin_9; 			// we are going to use PB8 and PB9
		GPIO_InitStruct.GPIO_Mode 		= GPIO_Mode_AF;						// set pins to alternate function
		GPIO_InitStruct.GPIO_Speed 		= GPIO_Speed_50MHz;					// set GPIO speed
		GPIO_InitStruct.GPIO_OType 		= GPIO_OType_OD;					// set output to open drain --> the line has to be only pulled low, not driven high
		GPIO_InitStruct.GPIO_PuPd 		= GPIO_PuPd_UP;						// enable pull up resistors
		GPIO_Init(GPIOB, &GPIO_InitStruct);									// init GPIOB

		// Connect I2C1 pins to AF  
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);	// SCL
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1); // SDA

		// configure I2C1 
		I2C_DeInit(I2Cx);
		I2C_InitStruct.I2C_ClockSpeed 			= speed; 						// 100kHz
		I2C_InitStruct.I2C_Mode 				= I2C_Mode_I2C;					// I2C mode
		I2C_InitStruct.I2C_DutyCycle 			= I2C_DutyCycle_2;				// 50% duty cycle --> standard
		I2C_InitStruct.I2C_OwnAddress1 			= ownAddress<<1;				// own address, not relevant in master mode
		I2C_InitStruct.I2C_Ack 					= I2C_Ack_Enable;				// disable acknowledge when reading (can be changed later on)
		I2C_InitStruct.I2C_AcknowledgedAddress 	= I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
		I2C_Init(I2Cx, &I2C_InitStruct);										// init I2C1

		//
		// Enable the I2C event & error interrupts.
		//
		I2C_ITConfig(I2Cx, I2C_IT_BUF, DISABLE); 	// enable the I2Cx event interrupt 
		I2C_ITConfig(I2Cx, I2C_IT_EVT, ENABLE); 	// enable the I2Cx event interrupt 
		I2C_ITConfig(I2Cx, I2C_IT_ERR, ENABLE); 	// disable the I2Cx error interrupt 

		// enable I2C1
		I2C_Cmd(I2Cx, ENABLE);

		//
		// Turn on clock stretching.
		//
		I2Cx->CR1 	&= (~0x80);

		//
		// Set ACK generation.
		//
		I2Cx->CR1 	|= 0x400;
	}


	//
	//
	//
	bool 			queueTooSmallFlag;
	TxQueueType& 	txQueue;
	RxQueueType& 	rxQueue;
	I2C_TypeDef*	I2Cx;

	I2CState 		state;
	uint8_t			subState;

	uint8_t 		receivedData;
	uint16_t		address;
	uint8_t 		direction;

	uint16_t		errorFlags;

	I2CCommand 		currentCommand;

	uint8_t* 		slaveResponseData;
	uint16_t		numberOfResponseBytes;

	EventEngineType& 	eventEngine;
	bool 				eventQTooSmallFlag;

	bool 			slaveMode;
	bool 			busBusy;
	bool 			receiveMode;
	bool 			byteTransferFinished;

	MethodHandler<I2C>      respondToSlaveReadHandler;
	MethodHandler<I2C>      respondToSlaveWriteHandler;
	MethodHandler<I2C>		slaveModeSTOPHandler;
	MethodHandler<I2C>		addressMatchedHandler;

	uint32_t 				slaveReadDelay;
};



#endif







