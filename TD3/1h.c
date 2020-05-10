#include <sys/mman.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  char cmd[64];
  char *buffer;

  /* on demande un mmap dans la deuxieme page, il le met un peu plus loin */
  buffer = mmap((void*) 0x1000, 4096, PROT_READ, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
  assert(buffer != MAP_FAILED);
  printf("mmap à %p (%ld) au lieu de %p (%ld)\n", buffer, (unsigned long) buffer, (void *) 0x1000UL, 0x1000UL);

  sprintf(cmd, "cat /proc/%u/maps", getpid());
  system(cmd);

  /* on demande un mmap dans la premiere page, il le met où il veut */
  /* on peut pas utiliser NULL ou 0x0 car cette valeur signifie "mmap n'importe ou".
   * en pratique, le 0x1 est aligné sur un debut de page, donc il devient 0x0 et donc "n'importe ou"
   */
  buffer = mmap((void*) 0x1, 4096, PROT_READ, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
  assert(buffer != MAP_FAILED);
  printf("mmap à %p (%ld) au lieu de %p (%ld)\n", buffer, (unsigned long) buffer, (void *) 0x1UL, 0x1UL);

  sprintf(cmd, "cat /proc/%u/maps", getpid());
  system(cmd);

  return 0;
}
