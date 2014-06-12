

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
	Queue() :
		head(0),
		tail(0)
	{
		//memset(&elements[0], '_', sizeof(elements) );
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
	    IndexType next_head = (head + 1) % NumberOfElements;
	    if (next_head != tail) 
	    {
	        /* there is room */
	        elements[head] = element;
	        head = next_head;
	        elementDroppedFlag 	= false;
	    } 
	    else 
	    {
	        /* no room left in the buffer */
	        elementDroppedFlag 	= true;
	    }
	}

	//
	// Remove element from the queue.
	//
	ElementType Get(bool& dataAvailableFlag)
	{
		ElementType 	c;

	    if (head != tail) 
	    {
	        c = elements[tail];
	        tail = (tail + 1) % NumberOfElements;
	        dataAvailableFlag 	= true;
	    } 
	    else 
	    {
	    	dataAvailableFlag	= false;
	    }
	    
        return c;
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
		}

		return returnValue;
	}


private:	

	//
	//
	//
	ElementType		elements[NumberOfElements];
	IndexType		head;
	IndexType		tail;

};



#endif







