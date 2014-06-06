

#ifndef __UART_H__
#define __UART_H__





//
//
//
template <	uint32_t Port,
			typename TxQueueType, 
			typename RxQueueType,
			uint32_t BaudRate>
class UART
{
public:

	//
	// Initialise the queue.
	//
	UART(TxQueueType& _txQueue, RxQueueType& _rxQueue) :
		txQueue(_txQueue),
		rxQueue(_rxQueue),
		queueTooSmallFlag(false)
	{
		initialise();
	}



	//
	// Process the master command queue.
	//
	bool ProcessQueue()
	{
		if(txQueue.IsEmpty() == false)
		{
			//
			// Let the ISR empty the queue.
			//
			return true;
		}

		return false;
	}




	//
	//
	//
	void ISR(void)
	{
		// Rx buffer not empty.
		//if( USART_GetITStatus(usart, USART_IT_RXNE) )
		{

			//
			// Data waiting in the UART Rx buffer, get it and put it in the queue
			// for the AppLevel to process.
			//
			char 	receivedByte = usart->DR;
			rxQueue.Put(receivedByte, queueTooSmallFlag);
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
	}


	//
	//
	//
	bool 			queueTooSmallFlag;
	TxQueueType& 	txQueue;
	RxQueueType& 	rxQueue;

};



#endif







