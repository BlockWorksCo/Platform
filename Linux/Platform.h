



#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "pthread.h"
#include <stdio.h>
#include <stdint.h>

//
// Platform name.
// 
#define LINUX



//
// User-visible macros.
//
#define START_RUNNABLE_IN_NEW_THREAD(obj)   pthread_create(&throwawayThreadID, NULL, StartRunnable, (void*)&obj);
#define START_RUNNABLE_IN_THIS_THREAD(obj)  while(true) (obj).Run()
#define DELAY_MS(ms)                        msleep(ms)
#define QUIT                                exit(0)



//
// Entry point.
//
extern "C" void AppMain();


//
// Necessary functions & variables to make the macros work.
//
extern pthread_t   throwawayThreadID;
void* StartRunnable(void* arg);
int msleep(unsigned int msec);


#endif




