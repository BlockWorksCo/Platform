

#include "Platform.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <msp430.h>
#include <legacymsp430.h>



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
    WDTCTL  = WDTPW + WDTHOLD; 	// Stop WDT

    do_ctors();

	//BCSCTL1 = CALBC1_1MHZ;      // Set DCO
  	//DCOCTL  = CALDCO_1MHZ;
 
    //printf("PlatformMain started.\n");
  	P1DIR |= 0x01;

#if 0
	P1OUT |= 0x01;
	P1OUT &= ~0x01;

	P1OUT |= 0x01;
	P1OUT &= ~0x01;

	P1OUT |= 0x01;
	P1OUT &= ~0x01;

	P1OUT |= 0x01;
	P1OUT &= ~0x01;

  	while(1)
  	{
	  	P1OUT ^= 0x01;
	  	for(unsigned int i=0; i<20000; i++);
  	}
#endif

    //
    // Start the application.
    //
    AppMain();

    return 0;
}



