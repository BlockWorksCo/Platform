



#ifndef __SOFTWAREUART_H__
#define __SOFTWAREUART_H__




template < 	typename RxPinType,
			typename TxPinType >
class SoftwareUART
{
public:

	SoftwareUART( RxPinType& _rxPin, TxPinType& _txPin ) :
		rxPin(_rxPin),
		txPin(_txPin)
	{
		
	}


	//
	//
	//
	void Tick()
	{
		bool 	rxLevel 	= rxPin.Get();
		txPin.Set();
	}

private:

	//
	//
	//
	RxPinType& 	rxPin;
	TxPinType& 	txPin;

};


#endif



