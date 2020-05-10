#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>

#define TAB_SIZE 65536

char path[64];

static void f() {
  static int i=1;
  char t[TAB_SIZE];
  printf("Profondeur=%d, ma pile est maintenant à %p\n", i++,  t);
  f();
}

void affiche_maps(int sig){
    printf("Mes maps sont maintenant:\n");
    system(path);
    exit(-1);
}

int main()
{
  /* Mise en place d'une pile alternative pour gérer les signaux lorsque la pile par défaut est pleine*/
  stack_t ss;
  void *ptr;
  ptr=malloc(SIGSTKSZ); /* alloue de la mémoire pour cette nouvelle pile afin que le gestionaire de signal puisse s'exécuter dessus*/
  ss.ss_sp=ptr;
  ss.ss_flags=0;
  ss.ss_size=SIGSTKSZ;
  sigaltstack(&ss,NULL);
  
  /* mise en place du deroutement*/
  struct sigaction act;
  act.sa_handler=affiche_maps; /*défini le gestionaire de signal*/
  act.sa_flags=SA_ONSTACK;  /* dit au système d'exécuter le gestionnaire sur la pile allouée par sigaltstack*/
  sigaction(SIGSEGV,&act,NULL);

  /* Récupère la taille maximale de la pile*/
  struct rlimit rlim;
  getrlimit(RLIMIT_STACK,&rlim); 
  printf("\nTaille pile max: %d (%d maximum appels recursifs pour f())\n\n",rlim.rlim_cur,rlim.rlim_cur/TAB_SIZE-1);

  /* défini la comande pour afficher  /proc/pid/maps */
  snprintf(path, sizeof(path), "cat /proc/%d/maps", getpid());
  printf("Mes maps sont:\n");
  system(path);

  /*go and call the recursive function*/
  printf("\nAppuyez sur entrée pour trouver le bas de la pile...\n");
  getchar();
  f();

  return 0;
}
