#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(){

  size_t length=1;
  void *ptr;
  long int i,size;

  while(1){
    ptr=mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);
    if(ptr==MAP_FAILED){
      printf("mmap failed with length: %f Go\n",length/(1024.0*1024.0*1024.0));
      break;
    }
    munmap(ptr,length);
    length*=2;
  }

  while(1){
    ptr=mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON|MAP_NORESERVE,-1,0);
    if(ptr==MAP_FAILED){
      printf("mmap MAP_NORESERVE failed with length: %f Go\n",length/(1024.0*1024.0*1024.0));
      break;
    }
    munmap(ptr,length);
    length*=2;
  }

  length=1024*1024*1024l;
  i=0;
  while(1){
    i++;
    ptr=mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON|MAP_NORESERVE,-1,0);
    if(ptr==MAP_FAILED){
      printf("mmap failed after %ld mmap of 1 Go. Total: %.3f To\n",i-1,(i-1)/1024.0);
      size=(i-1)*length;
      printf("nb bits: %d\n",(int)(ceil(log(size)/log(2))));
      exit(-1);
    }
  }
}
