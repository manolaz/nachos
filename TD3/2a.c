#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define FILENAME "/tmp/foobar"
#define LENGTH 8192

void show(char *map)
{
  printf("%c%c%c%c%c%c%c%c", map[0], map[1], map[2], map[3], map[4], map[5], map[6], map[7]);
}

int main(int argc, char *argv[])
{
  int fd;
  char zeros[1024];
  unsigned i;
  char *map1, *map2, *map3;
  char cmd[64];

  sprintf(cmd, "cat /proc/%u/maps", getpid());

  /* on cree un fichier rempli de 8ko de zero */
  fd = open(FILENAME, O_RDWR|O_CREAT);
  memset(zeros, '0', sizeof(zeros));
  for(i=0; i<(LENGTH+sizeof(zeros)-1)/sizeof(zeros); i++)
    write(fd, zeros, sizeof(zeros));

  system(cmd);

  map1 = mmap(NULL, LENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  assert(map1 != MAP_FAILED);
  map2 = mmap(NULL, LENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  assert(map2 != MAP_FAILED);
  map3 = mmap(NULL, LENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  assert(map3 != MAP_FAILED);

  system(cmd);
  /* on detruit le fichier sans le fermer, il n'existe plus que dans ce processus */
  unlink(FILENAME);
  /* maintenant les maps disent que le fichier a été effacé. ca permet d'etre sur qu'aucun
   * autre process pourra le modifier. seuls les mecs qui l'ont ouvert avant la suppression
   * y ont desormais acces. */
  system(cmd);

  printf("on écrit \"1111\" au début du mmap1\n");
  memcpy(map1, "1111", 4);
  printf("on écrit \"2222\" plus loin dans mmap2\n");
  memcpy(map2+4, "2222", 4);

  printf("map1 contient: \"");
  show(map1);
  printf("\"\n");

  printf("map2 contient: \"");
  show(map2);
  printf("\"\n");

  printf("map3 contient: \"");
  show(map3);
  printf("\"\n");

  munmap(map1, LENGTH);
  munmap(map2, LENGTH);
  munmap(map3, LENGTH);

  close(fd);
  return 0;
}
