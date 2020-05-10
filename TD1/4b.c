#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THRESHOLD (2.0/1000) /*2 ms*/

double get_time(){
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return 1.0*tv.tv_sec+(1.0*tv.tv_usec)/1e6;
}

int main(int argc, char **argv){
  double start,end1,end2;
  int pid;
  unsigned long n=0;
  double s_sum=0.0,r_sum=0.0;

  if (argc > 1) {
    unsigned seconds = atoi(argv[1]);
    printf("sleeping %d seconds\n", seconds);
    sleep(seconds);
  }

  pid=getpid();
  end2=start=get_time();
  while(n<20){
    end1=get_time();
    if(end1-end2>THRESHOLD){
      printf("%d -- SSS: %.2f ms\n",pid,1000*(end1-end2));
      printf("%d -- RRR: %.2f ms\n",pid,1000*(end2-start));
      s_sum+=1000*(end1-end2);
      r_sum+=1000*(end2-start);
      n++;
      start=end1;
      }
    end2=get_time();
    if(end2-end1>THRESHOLD){
      printf("%d -- SSS: %.2f ms\n",pid,1000*(end2-end1));
      printf("%d -- RRR: %.2f ms\n",pid,1000*(end1-start));
      s_sum+=1000*(end2-end1);
      r_sum+=1000*(end1-start);
      n++;
      start=end2;
    }
  }
  printf("%d -- Avg S: %.2f\n",pid,s_sum/n);
  printf("%d -- Avg R: %.2f\n",pid,r_sum/n);

  return 0;
}
