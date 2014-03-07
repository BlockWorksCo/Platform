



#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <windows.h>
#include <stdio.h>
#include <stdint.h>




//
// User-visible macros.
//
#define START_RUNNABLE_IN_NEW_THREAD(obj)   CreateThread(&throwawayThreadID, NULL, StartRunnable, (void*)&obj);
#define START_RUNNABLE_IN_THIS_THREAD(obj)  while(true) (obj).Run()
#define DELAY_MS(ms)                        Sleep(ms)
#define QUIT                                exit(0)



//
// Entry point.
//
extern "C" void AppMain();


//
// Necessary functions & variables to make the macros work.
//
extern HANDLE   throwawayThreadID;
void* StartRunnable(void* arg);


#endif




