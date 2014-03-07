



#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"

//
// Platform Name.
//
#define STM32



//
// User-visible macros.
//
#define START_RUNNABLE_IN_NEW_THREAD(obj)   
#define START_RUNNABLE_IN_THIS_THREAD(obj)  while(true) (obj).Run()
#define DELAY_MS(ms)                        
#define QUIT                                while(true)



//
// Entry point.
//
extern "C" void AppMain();


//
// Necessary functions & variables to make the macros work.
//


#endif




