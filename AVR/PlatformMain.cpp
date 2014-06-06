

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




typedef void(*ctor_t)(); 
   extern ctor_t __ctors_start; 
   extern ctor_t __ctors_end; 
 

void do_ctors()
{
   ctor_t ctor = __ctors_start; 
   while( ctor < __ctors_end ) 
   { 
     ctor(); 
     ctor = (ctor_t)((void*)ctor + sizeof(void*)); 
   } 
}




//
//
//
int main()
{
	init();
	do_ctors();

    //
    // Start the application.
    //
    AppMain();

    return 0;
}



