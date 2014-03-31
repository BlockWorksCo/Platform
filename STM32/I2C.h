

#ifndef __I2C_H__
#define __I2C_H__



#include <stm32f4xx_conf.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <stm32f4xx_i2c.h>

#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <stm32f4xx_usart.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src



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
			uint16_t ownAddress>
class I2C
{
public:

	//
	// Initialise the queue.
	//
	I2C(TxQueueType& _txQueue, RxQueueType& _rxQueue) :
		txQueue(_txQueue),
		rxQueue(_rxQueue),
		I2Cx((I2C_TypeDef*)PortBase),
		queueTooSmallFlag(false),
		state(i2cIdle),
		subState(0)
	{
		initialise();
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

	/* This function issues a start condition and 
	 * transmits the slave address + R/W bit
	 * 
	 * Parameters:
	 * 		I2Cx --> the I2C peripheral e.g. I2C1
	 * 		address --> the 7 bit slave address
	 * 		direction --> the tranmission direction can be:
	 * 						I2C_Direction_Tranmitter for Master transmitter mode
	 * 						I2C_Direction_Receiver for Master receiver
	 */
	void Start(uint8_t _address, uint8_t _direction)
	{
		address 	= _address;
		direction 	= _direction;

		// wait until I2C1 is not busy anymore
		{
			uint32_t i=0; 		
			while( (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) && (i<10000)) i++;
		}
		//BusyWaitTillIdle();
	  
		// Send I2C1 START condition 
		I2C_GenerateSTART(I2Cx, ENABLE);

		// wait for I2C1 EV5 --> Slave has acknowledged start condition
#if 0		
		{
			uint32_t i=0; 		
			while((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) && (i<10000)) i++;
		}
		//BusyWaitTillIdle();
#endif
		// Send slave Address for write 
		I2C_Send7bitAddress(I2Cx, address, direction);

		/* wait for I2C1 EV6, check if 
		 * either Slave has acknowledged Master transmitter or
		 * Master receiver mode, depending on the transmission
		 * direction
		 */ 
		//BusyWaitTillIdle();
#if 0		 
		if(direction == I2C_Direction_Transmitter)
		{
			{
				uint32_t i=0; 		
				while((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (i<10000)) i++;
			}
		}
		else if(direction == I2C_Direction_Receiver)
		{
			{
				uint32_t i=0; 		
				while((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && (i<10000)) i++;
			}
		}
#endif		
	}


	/* This function transmits one byte to the slave device
	 * Parameters:
	 *		I2Cx --> the I2C peripheral e.g. I2C1 
	 *		data --> the data byte to be transmitted
	 */
	void Write(uint8_t data)
	{
		I2C_SendData(I2Cx, data);
		// wait for I2C1 EV8_2 --> byte has been transmitted
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}


	/* This function reads one byte from the slave device 
	 * and acknowledges the byte (requests another byte)
	 */
	uint8_t ReadACK()
	{
		// enable acknowledge of recieved data
		I2C_AcknowledgeConfig(I2Cx, ENABLE);
		// wait until one byte has been received
		while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
		// read data from I2C data register and return data byte
		uint8_t data = I2C_ReceiveData(I2Cx);
		return data;
	}

	/* This function reads one byte from the slave device
	 * and doesn't acknowledge the recieved data 
	 */
	uint8_t ReadNACK()
	{
		// disabe acknowledge of received data
		// nack also generates stop condition after last byte received
		// see reference manual for more info
		I2C_AcknowledgeConfig(I2Cx, DISABLE);
		I2C_GenerateSTOP(I2Cx, ENABLE);
		// wait until one byte has been received
		while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
		// read data from I2C data register and return data byte
		uint8_t data = I2C_ReceiveData(I2Cx);
		return data;
	}

	/* This funtion issues a stop condition and therefore
	 * releases the bus
	 */
	void Stop()
	{
		// Send I2C1 STOP Condition 
		I2C_GenerateSTOP(I2Cx, ENABLE);
	}




	//
	//
	//
	bool ProcessQueue()
	{
		bool		dataAvailableFlag	= false;
		I2CCommand 	command				= rxQueue.Get(dataAvailableFlag);

		if(dataAvailableFlag == true)
		{
			switch(command.type)
			{

				case i2cMasterWrite:
				{
					//
					// I2C Master Write
					//
					Start(command.target<<1, I2C_Direction_Transmitter);
					for(uint32_t i=0; i<command.numberOfBytes; i++)
					{
						Write(command.payload[i]);
					}
					Stop();

					break;
				}


				case i2cMasterRead:
				{
					uint32_t i;

					//
					// I2C Master Read
					//
					Start(command.target<<1, I2C_Direction_Receiver); 		// start a transmission in Master receiver mode
					for(i=0; i<command.numberOfBytes-1; i++)
					{
						command.payload[i] = ReadACK(); // read one byte and request another byte
					}
					command.payload[i] = ReadNACK();	// read one byte and don't request another byte, stop transmission

					break;
				}


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
		uint16_t 	sr1			= I2Cx->SR1;
		uint16_t 	sr2			= I2Cx->SR2;
		bool 		slaveMode	= false;

		if( (sr2&0x0001) == 0)		// MSL bit.
		{
			slaveMode	= true;
		}

		//
		//
		//
		switch(state)
		{
			case i2cStart:
			{
				//
				// Start generation.
				//
				switch(subState)
				{
					case 0:
					{
						if( I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == false)
						{
							// Send I2C1 START condition 
							I2C_GenerateSTART(I2Cx, ENABLE);
							subState++;
						}
						break;							
					}

					case 1:
					{
						if( I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) == false)
						{
							// Send slave Address for write 
							I2C_Send7bitAddress(I2Cx, address, direction);

							subState++;
						}
						break;							
					}

					case 2:
					{
						if(			(direction == I2C_Direction_Transmitter) &&
									(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == false))
						{
							state 	= i2cIdle;
						}
						else if(	(direction == I2C_Direction_Receiver) && 
									( I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == false ))
						{
							state 	= i2cIdle;
						}
						break;							
					}

					default:
					{
						break;
					}
				}

				break;
			}


			case i2cWrite:
			{
				//
				// MasterWrite.
				//

				//I2C_SendData(I2Cx, data);

				// wait for I2C1 EV8_2 --> byte has been transmitted
				if(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == false)
				{
					state 	= i2cIdle;
				}


				break;
			}


			case i2cReadACK:
			{
				//
				// ReadACK.
				//

				// enable acknowledge of recieved data
				//I2C_AcknowledgeConfig(I2Cx, ENABLE);
				// wait until one byte has been received
				if( I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == false)
				{
					// read data from I2C data register and return data byte
					receivedData = I2C_ReceiveData(I2Cx);
					state 	= i2cIdle;
				}

				break;
			}


			case i2cReadNACK:
			{
				//
				// ReadNACK.
				//

				// disable acknowledge of received data
				// nack also generates stop condition after last byte received
				// see reference manual for more info
				//I2C_AcknowledgeConfig(I2Cx, DISABLE);
				//I2C_GenerateSTOP(I2Cx, ENABLE);
				// wait until one byte has been received
				if( I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == false)
				{
					receivedData = I2C_ReceiveData(I2Cx);
					state 	= i2cIdle;
				}

				break;
			}


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
					state = i2cIdle;
				}

				if( (sr1&0x0080) != 0)		// TxE bit
				{
					//
					// Slave mode address acknowledged.
			        // The slave stretches SCL low until ADDR is
			        // cleared and DR filled with the data to be sent
					//
			        I2Cx->DR 	= 0xab;
					state = i2cIdle;
				}

#if 0
 switch (I2C_GetLastEvent(I2C2))
    {
    //The address sent by the master matches the own address of the peripheral
    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
        //The slave stretches SCL low until ADDR is
        //cleared and DR filled with the data to be sent
        I2C_ClearFlag(I2C2,I2C_FLAG_ADDR);
        break;

    //The application is expecting a data byte to be received
    case I2C_EVENT_SLAVE_BYTE_RECEIVED:
        I2C_ReceiveData(I2C2);
        break;

    //The application is expecting the end of the communication
    //Make sure that both ADDR and STOPF flags are cleared
    //if both are found set.
    case I2C_EVENT_SLAVE_STOP_DETECTED:
        if(I2C_GetFlagStatus(I2C2,I2C_FLAG_ADDR) == SET)
            I2C_ClearFlag(I2C2,I2C_FLAG_ADDR);
        if(I2C_GetFlagStatus(I2C2,I2C_FLAG_STOPF) == SET)
            I2C_ClearFlag(I2C2,I2C_FLAG_STOPF);
}
#endif				
			}


			default:
			{
				break;
			}
		}

	}


	//
	//
	//
	void EventISR(void)
	{
		StateMachine(state,	subState);
#if 0		
		bool		dataAvailableFlag	= false;
		I2CCommand 	command				= rxQueue.Peek(dataAvailableFlag);

		if(dataAvailableFlag == true)
		{			
			//
			// Process events.
			//
			StateMachine(state,	subState);
		}
		else
		{
			//
			// Error? Event without command in progress.
			//
			state = i2cIdle;
		}
#endif		
	}

	//
	// Errors can be:
	// - Buss Error
	// - Arbitration Loss.
	// - Ack Failure.
	// - Over/Under run.
	//
	void ErrorISR(void)
	{
		//
		// Check on I2C1 AF flag and clear it
		//
		if (I2C_GetITStatus(I2Cx, I2C_IT_AF)) 
		{
			I2CCommand	command;

			//
			// Form a command and put it in the I2C receive queue.
			//
			command.type 			= i2cError;
			command.payload[0] 		= 0;
			command.numberOfBytes 	= 0;
			txQueue.Put(command, queueTooSmallFlag);

			//
			// Clear the condition.
			//
			I2C_ClearITPendingBit(I2Cx, I2C_IT_AF);
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
		I2C_InitStruct.I2C_OwnAddress1 			= ownAddress;					// own address, not relevant in master mode
		I2C_InitStruct.I2C_Ack 					= I2C_Ack_Enable;				// disable acknowledge when reading (can be changed later on)
		I2C_InitStruct.I2C_AcknowledgedAddress 	= I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
		I2C_Init(I2Cx, &I2C_InitStruct);										// init I2C1

		//
		// Enable the I2C event & error interrupts.
		//
		I2C_ITConfig(I2Cx, I2C_IT_EVT, ENABLE); 	// enable the I2Cx event interrupt 
		I2C_ITConfig(I2Cx, I2C_IT_ERR, ENABLE); 	// disable the I2Cx error interrupt 

		// enable I2C1
		I2C_Cmd(I2Cx, ENABLE);
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

};



#endif







