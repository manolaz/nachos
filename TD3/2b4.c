#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
  int fd;
  char *map1, *map2, *map3;
  char buffer[100];

  system("dd if=/dev/zero of=/tmp/foobar count=2048 bs=512");
  fd = open("/tmp/foobar", O_RDWR);
  if (fd < 0) {
    perror("open");
    exit(-1);
  }

  map1 = mmap(NULL, 1024*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (map1 == MAP_FAILED) {
    perror("mmap 1");
    exit(-1);
  }
  map2 = mmap(NULL, 1024*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (map2 == MAP_FAILED) {
    perror("mmap 2");
    exit(-1);
  }
  map3 = mmap(NULL, 1024*1024, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (map3 == MAP_FAILED) {
    perror("mmap 3");
    exit(-1);
  }
  printf("pid: %d shared %p shared %p private %p\n", getpid(), map1, map2, map3);
 
  printf("appuyez sur entrée\n");
  getchar();

  printf("remplissage du shared1 avec des a\n");
  memset(map1, 'a', 1024*1024);
  printf("remplissage du début du shared2 avec des b\n");
  memset(map2, 'b', 50);
  printf("lecture du début du shared1\n");
  memcpy(buffer, map1, 100);
  buffer[98] = '\n';
  buffer[99] = 0;
  printf(buffer);

  printf("lecture du début du private3\n");
  memcpy(buffer, map3, 100);
  buffer[98] = '\n';
  buffer[99] = 0;
  printf(buffer);

  printf("ecrasement du debut du private3\n");
  memset(map3, 'c', 10);
  printf("lecture du début du private3 (dupliquée ci-dessus)\n");
  memcpy(buffer, map3, 100);
  buffer[98] = '\n';
  buffer[99] = 0;
  printf(buffer);

  printf("lecture du début du shared1\n");
  memcpy(buffer, map1, 100);
  buffer[98] = '\n';
  buffer[99] = 0;
  printf(buffer);

  printf("Ecrit la fin du shared1 avec des d\n");
  memset(map1+1024*1024-20,'d',20);     ;

  printf("lecture de la fin du shared1\n");
  memcpy(buffer, map1+1024*1024-100, 100);
  buffer[98] = '\n';
  buffer[99] = 0;
  printf(buffer);

  printf("lecture de la fin du private3 (pas encore dupliquée)\n");
  memcpy(buffer, map3+1024*1024-100, 100);
  buffer[98] = '\n';
  buffer[99] = 0;
  printf(buffer);

  printf("Ecrit la fin du private3 avec des e\n");
  memset(map3+1024*1024-10,'e',10);     ;

  printf("lecture de la fin du shared1\n");
  memcpy(buffer, map1+1024*1024-100, 100);
  buffer[98] = '\n';
  buffer[99] = 0;
  printf(buffer);

  printf("lecture de la fin du private3 (enfin dupliquée)\n");
  memcpy(buffer, map3+1024*1024-100, 100);
  buffer[98] = '\n';
  buffer[99] = 0;
  printf(buffer);

  
  printf("Ecrit a cheval sur 1e et 2e page de shared1\n");
  memset(map1+4086, 'f', 20);

  printf("lecture à cheval 1e et 2e page de shared1\n");
  memcpy(buffer, map1+4076, 100);
  buffer[98] = '\n';
  buffer[99] = 0;
  printf(buffer);

  printf("lecture à cheval 1e et 2e page private3 (à moitié dupliqué)\n");
  memcpy(buffer, map3+4076, 100);
  buffer[98] = '\n';
  buffer[99] = 0;
  printf(buffer);

  unlink("/tmp/foobar");

  return 0;
}
