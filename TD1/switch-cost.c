#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>

int main() {
    int n = 1000000;
  struct timeval tv[2];
  unsigned long us;
  unsigned i;
  while (1) {
    gettimeofday(&tv[0], NULL);
    for(i=0; i<n; i++)
      sched_yield();
    gettimeofday(&tv[1], NULL);
    us = (tv[1].tv_sec-tv[0].tv_sec)*n+(tv[1].tv_usec-tv[0].tv_usec);
    printf("sched_yield took %fns\n", us*1.0*1000/n);
  }
  return 0;
}
