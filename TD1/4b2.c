#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>


int main(){
    const int myPid = getpid();
    const unsigned long nbLoopToWait = 500000;
    struct timeval timers[2];
    unsigned long elapsedLoopTime;
    unsigned long minCostOfGetOneLoop = ULONG_MAX;

    int idxTime = 1;
    timers[!idxTime].tv_sec = 0;
    timers[!idxTime].tv_usec = 0;

    while(1){
	    for(unsigned long idx = 0 ; idx < nbLoopToWait ; ++idx){
		 struct timeval timerUnused;
                 gettimeofday(&timerUnused, NULL);
	    }

            gettimeofday(&timers[idxTime], NULL);

            elapsedLoopTime = (timers[idxTime].tv_sec-timers[!idxTime].tv_sec)*1000000
                             +(timers[idxTime].tv_usec-timers[!idxTime].tv_usec);
            idxTime = !idxTime;

            if(elapsedLoopTime < minCostOfGetOneLoop ){
                 minCostOfGetOneLoop = elapsedLoopTime;
            }

            printf("\rProcess %d has been executed %6lu with %6lu out => %4.3lf of min exec time",
		   myPid, minCostOfGetOneLoop, elapsedLoopTime-minCostOfGetOneLoop, (double)elapsedLoopTime/(double)minCostOfGetOneLoop);
    }

    return 0;
}
