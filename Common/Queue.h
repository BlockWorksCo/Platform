

#ifndef __QUEUE_H__
#define __QUEUE_H__





//
//
//
template <	typename ElementType, 
			uint32_t NumberOfElements, 
			typename IndexType>
class Queue
{
public:

	//
	// Initialise the queue.
	//
	Queue()
	{
		head					= 0;
		tail 					= 0;
		numberOfElementsInQueue	= 0;
	}

	//
	//
	//
	bool IsEmpty()
	{
		if( head != tail)
		{
			return false;
		}

		return true;
	}

	//
	// Put an element in the queue.
	//
	void Put(ElementType element, bool& elementDroppedFlag)
	{
		IndexType 	nextHead	= (head+1) % NumberOfElements;

		elementDroppedFlag	= false;		

		if( nextHead != tail)
		{
			elements[head] 	= element;
			head 			= nextHead;

			numberOfElementsInQueue++;			
		}
		else
		{
			//
			// Queue has filled up and become lossy.
			//
			elementDroppedFlag	= true;
		}
	}

	//
	// Remove element from the queue.
	//
	ElementType Get(bool& dataAvailableFlag)
	{
		ElementType returnValue		= elements[tail];

		dataAvailableFlag		= false;	

		if(IsEmpty() == false)
		{
			returnValue			= elements[tail];
			tail 				= (tail + 1) % NumberOfElements;	
			dataAvailableFlag	= true;		

			numberOfElementsInQueue--;
		}

		return returnValue;
	}

	//
	// Peek at (but dont remove) the element from the queue.
	//
	ElementType Peek(bool& dataAvailableFlag)
	{
		ElementType returnValue		= elements[tail];

		dataAvailableFlag		= false;	

		if(tail != head)
		{
			returnValue			= elements[tail];
			dataAvailableFlag	= true;		

			numberOfElementsInQueue--;
		}

		return returnValue;
	}


	//
	//
	//
	IndexType NumberOfElementsInQueue()
	{
		return numberOfElementsInQueue;
	}

private:	

	//
	//
	//
	ElementType		elements[NumberOfElements];
	IndexType		head;
	IndexType		tail;
	IndexType		numberOfElementsInQueue;

};



#endif







