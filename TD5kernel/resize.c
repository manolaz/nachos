#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[])
{
  int newlength;
  int fd;
  int err;

  if (argc == 1) {
    printf("nothing to do\n");
    return 0;
  }
  newlength = atoi(argv[1]);
  printf("resizing to %d\n", newlength);

  fd = open("/dev/special", O_RDWR);
  if (fd < 0) {
    perror("open");
    return -1;
  }

  err = ioctl(fd, 1, newlength);
  if (err < 0) {
    perror("ioctl");
    return -1;
  }
  printf("ok\n");

  close(fd);
  return 0;
}
