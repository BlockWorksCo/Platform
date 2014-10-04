



#ifndef __SOFTWAREUART_H__
#define __SOFTWAREUART_H__




template < 	typename RxPinType,
			typename TxPinType >
class SoftwareUART
{
public:

	SoftwareUART()
	{
		
	}


	//
	//
	//
	void Tick()
	{
		
	}

private:

	//
	//
	//
	RxPinType 	rxPin;
	TxPinType 	txPin;

};


#endif



