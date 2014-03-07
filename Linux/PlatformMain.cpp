

#include "Platform.h"
//#include "IRunnable.h"
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <string.h>


pthread_t   throwawayThreadID;




//
// http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/
//
int msleep(unsigned int msec)
{
    struct timespec     timeout0;
    struct timespec     timeout1;
    struct timespec*    tmp;
    struct timespec*    t0          = &timeout0;
    struct timespec*    t1          = &timeout1;

    t0->tv_sec = msec / 1000;
    t0->tv_nsec = (msec % 1000) * (1000 * 1000);

    while(nanosleep(t0, t1) == -1)
    {
        if(errno == EINTR)
        {
            tmp = t0;
            t0  = t1;
            t1  = tmp;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}




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
void sigfunc(int sig_no)
{
}


//
//
//
int main()
{
    /*
    struct sigaction    sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler   = &sigfunc;
    sigaction(SIGINT, &sa,NULL);
    */

    //
    // Start the application.
    //
    AppMain();

    return 0;
}



