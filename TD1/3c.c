#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include <sched.h>


int main(){
    printf("\n");
    while(1){
        const unsigned nbLoops = 100000;
        unsigned long elapsedMicroseconds;
        struct timeval startTime, endTime;
        unsigned idxLoop;

        gettimeofday(&startTime, NULL);
        for( idxLoop = 0 ; idxLoop < nbLoops ; ++idxLoop){
            sched_yield();
        }
        gettimeofday(&endTime, NULL);


        elapsedMicroseconds = (endTime.tv_sec-startTime.tv_sec)*1000000 +(endTime.tv_usec-startTime.tv_usec);
        printf("\rCalling sched_yield() %u times took %8lu microseconds", nbLoops, elapsedMicroseconds);
    }

    return 0;
}
