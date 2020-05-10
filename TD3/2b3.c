#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define FILENAME "/tmp/foobar"
#define LENGTH 1024

int main(int argc, char *argv[])
{
  int fd;
  char zeros[1024];
  unsigned i;
  int isshared1 = 1, isshared2 = 1, isshared3 = 1;
  char *map1, *map2, *map3;
  char cmd[64];

  sprintf(cmd, "cat /proc/%u/maps", getpid());

  if (argc > 1) {
    if (atoi(argv[1]) == 1)
      isshared1 = 0;
    else if (atoi(argv[1]) == 2)
      isshared2 = 0;
    else if (atoi(argv[1]) == 3)
      isshared3 = 0;
  }

  /* on cree un fichier rempli de 1Mo de zero */
  fd = open(FILENAME, O_RDWR|O_CREAT);
  memset(zeros, 0, sizeof(zeros));
  for(i=0; i<(LENGTH+sizeof(zeros)-1)/sizeof(zeros); i++)
    write(fd, zeros, sizeof(zeros));

  system(cmd);

  printf("mmap1 %s\n", isshared1 ? "shared" : "private");
  map1 = mmap(NULL, LENGTH, PROT_READ|PROT_WRITE, isshared1 ? MAP_SHARED : MAP_PRIVATE, fd, 0);
  assert(map1 != MAP_FAILED);
  printf("map1 %p\n", map1);
  printf("mmap2 %s\n", isshared2 ? "shared" : "private");
  map2 = mmap(NULL, LENGTH, PROT_READ|PROT_WRITE, isshared2 ? MAP_SHARED : MAP_PRIVATE, fd, 0);
  assert(map2 != MAP_FAILED);
  printf("map2 %p\n", map2);
  printf("mmap3 %s\n", isshared3 ? "shared" : "private");
  map3 = mmap(NULL, LENGTH, PROT_READ|PROT_WRITE, isshared3 ? MAP_SHARED : MAP_PRIVATE, fd, 0);
  assert(map3 != MAP_FAILED);
  printf("map3 %p\n", map3);

  system(cmd);
  /* on detruit le fichier sans le fermer, il n'existe plus que dans ce processus */
  unlink(FILENAME);
  /* maintenant les maps disent que le fichier a été effacé. ca permet d'etre sur qu'aucun
   * autre process pourra le modifier. seuls les mecs qui l'ont ouvert avant la suppression
   * y ont desormais acces. */
  system(cmd);

  printf("on écrit \"hello \" au début du mmap1\n");
  memcpy(map1, "hello ", 6);
  printf("on écrit \"world!\" plus loin dans mmap2\n");
  memcpy(map2+6, "world!", 6);

  printf("map1 contient: \"%s\"\n", map1);
  printf("map2 contient: \"%s\"\n", map2);
  printf("map3 contient: \"%s\"\n", map3);

  munmap(map1, LENGTH);
  munmap(map2, LENGTH);
  munmap(map3, LENGTH);

  close(fd);
  return 0;
}
