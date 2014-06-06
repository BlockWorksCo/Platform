



#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdio.h>
#include <stdint.h>
#include <Arduino.h>



//
// Platform Name.
//
//#define MSP430


void Flash();
void Init();


//
// User-visible macros.
//
#define START_RUNNABLE_IN_THIS_THREAD(obj)  while(true) (obj).Run()
#define DELAY_MS(ms)                        for(volatile unsigned int i=0; i<ms*10; i++);
#define QUIT                                while(true)



//
// Entry point.
//
extern "C" void AppMain();


//
// Necessary functions & variables to make the macros work.
//



void DebugOut(uint8_t ch);


#endif




