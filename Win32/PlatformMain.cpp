

#include "Platform.h"
//#include "IRunnable.h"
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <string.h>


HANDLE   throwawayThreadID;






//
// Entry point for a thread that calls a Runnable object.
//
void* StartRunnable(void* arg)
{
    printf("StartRunnable...\n");
    //IRunnable*  runnable    = static_cast<IRunnable*>(arg);

    while(true)
    {
        //START_RUNNABLE_IN_THIS_THREAD(*runnable);
    }

    return NULL;
}


//
//
//
int main()
{
    printf("PlatformMain started.\n");

    //
    // Start the application.
    //
    AppMain();

    return 0;
}



