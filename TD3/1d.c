#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
  char *buffer;
  char cmd[64];
  char tmpfile[64] = "/tmp/mytmp.foobar.XXXXXX";
  pid_t pid = getpid();
  snprintf(cmd, sizeof(cmd), "cat /proc/%u/maps", pid);
  int fd;

  printf("######################### DEBUT ###################\n");
  system(cmd);

  buffer = mmap(NULL, 4096*4096, PROT_READ, MAP_SHARED | MAP_ANON, -1, 0);
  assert(buffer != MAP_FAILED);
  printf("######################### MMAP READ SHARED ANONYME ###################\n");
  system(cmd);

  buffer = mmap(NULL, 4096*4096, PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
  assert(buffer != MAP_FAILED);
  printf("######################### MMAP WRITE PRIVATE ANONYME ###################\n");
  system(cmd);

  fd = open("/home/bgoglin/.bazshrc", O_RDONLY);
  buffer = mmap(NULL, 4096, PROT_READ, MAP_PRIVATE, fd, 4096);
  assert(buffer != MAP_FAILED);
  printf("######################### MMAP READ PRIVATE FILE ###################\n");
  system(cmd);

  fd = mkstemp(tmpfile);
  unlink(tmpfile);
  buffer = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 4096);
  assert(buffer != MAP_FAILED);
  printf("######################### MMAP READ/WRITE SHARED DELETED FILE ###################\n");
  system(cmd);

  return 0;
}
