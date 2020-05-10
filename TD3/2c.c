#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define FILENAME "/tmp/foobar"
#define LENGTH (1024*1024)

int main(int argc, char *argv[])
{
  int fd;
  char zeros[1024];
  unsigned i;
  char *map;
  char c = 0;
  int steps = atoi(argv[1]);
  int reverse = 0;
  int populate = 0;
  int private = 0;

  if (steps >= 1000) {
    populate = 1;
    steps -= 1000;
  }
  if (steps >= 100) {
    reverse = 1;
    steps -= 100;
  }
  if (steps >= 10) {
    private = 1;
    steps -= 10;
  }

  if (steps == 0) {
    printf("on ne fait rien\n");
    return 0;
  }

  /* on cree un fichier rempli de 1Mo de zero */
  fd = open(FILENAME, O_RDWR|O_CREAT);
  memset(zeros, 0, sizeof(zeros));
  for(i=0; i<(LENGTH+sizeof(zeros)-1)/sizeof(zeros); i++)
    write(fd, zeros, sizeof(zeros));
  /* on detruit le fichier sans le fermer, il n'existe plus que dans ce processus */
  unlink(FILENAME);

  if (steps == 1) {
    printf("on arrete après open/unlink\n");
    return 0;
  }

  map = mmap(NULL, LENGTH, PROT_READ|PROT_WRITE, (private ? MAP_PRIVATE : MAP_SHARED) | (populate ? MAP_POPULATE : 0), fd, 0);
  assert(map != MAP_FAILED);

  if (steps == 2) {
    printf("on arrete après mmap\n");
    return 0;
  }

  if (reverse) goto write;
read:

  for(i=0; i<1; i++) c += map[i];

  if (steps == 3) {
    printf("on arrete après une lecture\n");
    return 0;
  }

  for(i=1; i<4096; i++) c += map[i];

  if (steps == 4) {
    printf("on arrete après 4096 lectures\n");
    return 0;
  }

  for(i=4096; i<LENGTH; i++) c += map[i];

  if (steps == 5) {
    printf("on arrete après toute la lecture\n");
    return 0;
  }

  if (reverse) goto end;
write:

  memset(map, 'a', 1);

  if (steps == 6) {
    printf("on arrete après une ecriture\n");
    return 0;
  }

  memset(map+1, 0, 4095);

  if (steps == 7) {
    printf("on arrete après 4096 ecriture\n");
    return 0;
  }

  memset(map+4096, 'a', LENGTH-4096);

  if (steps == 8) {
    printf("on arrete après toute l'ecriture\n");
    return 0;
  }

  if (reverse) goto read;
end:

  munmap(map, LENGTH);

  close(fd);
  printf("on fait tout\n");
  return 0;
}
