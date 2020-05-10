#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  struct timeval tv[2];
  unsigned long us;
  unsigned i;
  gettimeofday(&tv[0], NULL);
  for(i=0; i<1000000; i++) {
    gettimeofday(&tv[1], NULL);
  }
  us = (tv[1].tv_sec-tv[0].tv_sec)*1000000+(tv[1].tv_usec-tv[0].tv_usec);
  printf("1000000 gettimeofday took %ldus\n", us);
  return 0;
}
