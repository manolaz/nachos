#define _GNU_SOURCE 1
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>

int main(int argc, char *argv[])
{
  int fd[2];
  char *buffer;
  size_t len;
  ssize_t ret;

  if (argc <= 1) {
    printf("missing buffer length\n");
    exit(EXIT_FAILURE);
  }
  len = atoi(argv[1]);
  buffer = malloc(len);
  if (!buffer) {
    exit(EXIT_FAILURE);
  }

  ret = pipe(fd);
  assert(ret == 0);
  fcntl(fd[0], F_SETPIPE_SZ, 1048576);

  assert(len < 1048576);

  while (1) {
    /* attention le tube est unidirectionnel,
     * pas possible de lire dans fd[1] ou d'ecrire dans fd[0] */
    ret = write(fd[1], buffer, len);
    assert(ret == len);
    ret = read(fd[0], buffer, len);
    assert(ret == len);
  }
}
