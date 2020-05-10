#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <assert.h>

#define FILENAME "/tmp/foobar"

int main()
{
  int fd, err;
  char buf[50];
  pid_t pid;
  int status;

  fd = open(FILENAME, O_CREAT|O_EXCL|O_RDWR, S_IRWXU);

  if (fd<0) {
    perror("open");
    return -1;
  }

  err = flock(fd, LOCK_EX);
  assert(!err);
  printf("le père a pris le lock\n");

  pid = fork();
  if (pid) {
    /* pere */
    printf("le père attend 2 secondes\n");
    sleep(2);
    printf("le père relâche le lock\n");
    err = flock(fd, LOCK_UN);
    assert(!err);
    printf("le père attend 1s\n");
    sleep(1);
    printf("le père écrit sans le verrou\n");
    err = write(fd, buf, sizeof(buf));
    assert(err = sizeof(buf));
    printf("le père attend le fils\n");
    wait(&status);
  } else {
    /* fils */
    fd = open(FILENAME, O_RDWR);
    assert(fd >= 0);
    printf("  le fils essaie de prendre le lock\n");
    err = flock(fd, LOCK_EX);
    assert(!err);
    printf("  le fils a pris le lock\n");
    printf("  le fils dort 3 secondes\n");
    sleep(3);
    printf("  le fils relache le lock\n");
    err = flock(fd, LOCK_UN);
    assert(!err);
  }

  close(fd);
  unlink(FILENAME);
  return 0;
}
