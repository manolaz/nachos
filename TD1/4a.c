#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  struct timeval tv[2];
  unsigned long us;
  int i = 0;
  pid_t pid = getpid();
  gettimeofday(&tv[!i], NULL);
  while (1) {
    gettimeofday(&tv[i], NULL);
    us = (tv[i].tv_sec-tv[!i].tv_sec)*1000000+(tv[i].tv_usec-tv[!i].tv_usec);
    if (us >= 1000)
      printf("pid %d : %ld us\n", pid, us);
    i = !i;
  }
  return 0;
}
