

#include "Platform.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>





void Flash()
{
	pinMode(13, OUTPUT); 
	digitalWrite(13, HIGH);
	for(volatile uint32_t i=0; i<20000; i++);
	digitalWrite(13, LOW);
	for(volatile uint32_t i=0; i<20000; i++);
}


void Init()
{
	static bool initialised = false;

	if(initialised == false)
	{
		initialised = true;
		init();
	}	
}





//
//
//
int main()
{
	init();

    //
    // Start the application.
    //
    AppMain();

    return 0;
}



