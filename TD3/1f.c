#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

static char cmd[64];
static char *s2;

static void f(int i)
{
  char s[10000];
  if (!(i%100)) {
    printf("##### appel n°%u\n", i);
    fflush(stdout);
    system(cmd);
  }
  s2 = s;
  f(i+1);
}

void g(int sig) {
  printf("###################################\n");
  printf("dernier buffer = %p\n", s2);
  printf("###################################\n");
  fflush(stdout);
  system(cmd);
  exit(-1);
}

int main()
{
  struct sigaction act;
  stack_t ss;
  void *ptr;
  int err;

  ptr=malloc(SIGSTKSZ); /* alloue de la mémoire pour cette nouvelle pile afin que le gestionaire de signal puisse s'exécuter dessus*/
  ss.ss_sp=ptr;
  ss.ss_flags=0;
  ss.ss_size=SIGSTKSZ;
  err = sigaltstack(&ss,NULL);
  if (err < 0)
    perror("sigaltstack");

  act.sa_handler= g; /*défini le gestionaire de signal*/
  act.sa_flags=SA_ONSTACK;
  sigaction(SIGSEGV,&act,NULL);

  sprintf(cmd, "cat /proc/%u/maps", getpid());

  f(0);
  return 0;
}
