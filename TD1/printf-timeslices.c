#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct timeval tv[2], rescheduled;
  unsigned x;
  if (argc > 1) {
    unsigned seconds = atoi(argv[1]);
    printf("sleeping %d seconds\n", seconds);
    sleep(seconds);
  }
  pid_t pid = getpid();
  gettimeofday(&tv[0], NULL);
  memcpy(&rescheduled, &tv[0], sizeof(rescheduled));
  x = 1;
  while (1) {
    unsigned long notmyus, myus;
    gettimeofday(&tv[x], NULL);
    notmyus = (tv[x].tv_sec-tv[1-x].tv_sec)*1000000+(tv[x].tv_usec-tv[1-x].tv_usec);
    if (notmyus > 1000) {
      /* we ran from rescheduled to tv[x] and did not run from tv[x] to tv[1-x] */
      myus = (tv[1-x].tv_sec-rescheduled.tv_sec)*1000000+(tv[1-x].tv_usec-rescheduled.tv_usec);
      printf("\r%d - run during %ldus - did not run during %ldus", pid, myus, notmyus);
      memcpy(&rescheduled, &tv[x], sizeof(rescheduled));
    }
    x = 1-x;
  }
  return 0;
}
