#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>

int main() {
  char s[64];
  int i;
  char *buffer, *mmapbuffer;

  sprintf(s, "cat /proc/%u/maps", getpid());
  system(s);
  /* y a pas encore de tas ici */

  /* plein de petits mallocs dans le vrai tas du bas */
  for(i=0; i<10000; i++) {
    buffer = malloc(10000);
    if (!(i%1000))
      /* on affiche tous les 1000 */
      printf("#### petit malloc à %p\n", buffer);
  }

  fflush(stdout);
  system(s);
  /* le vrai tas a été créé */

  for(i=0; i<1000; i++) {
    buffer = malloc(1000000);
    if (!(i%100)) {
      /* on affiche tous les 100 */
      printf("#### gros malloc à %p\n", buffer);
      fflush(stdout);
      system(s);
      /* un autre "tas" a été créé pour ces gros mallocs */
    }
  }

  buffer = malloc(10000);
  printf("#### petit malloc à %p\n", buffer);
  fflush(stdout);
  system(s);
  /* ce petit malloc va dans le premier vrai tas pour les petits */

  /* on force un mmap au dessus du vrai petit tas pour forcer un debordement */
  /* pas le mettre trop pret, car s'il touche ce que malloc a réservé,
   * le noyau ne pourra pas honorer notre demande et va le mettre n'importe ou */
  /* pas le mettre trop loin sinon le test plus bas ne debordera */
  mmapbuffer = mmap(buffer + 1000000, 4096, PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  assert(mmapbuffer != MAP_FAILED);
  printf("##### mmap à %p (forcé à %p)\n", mmapbuffer, buffer + 1000000);
  fflush(stdout);
  system(s);

  for(i=0; i<10000; i++) {
    buffer = malloc(10000);
    if (!(i%1000))
      printf("#### petit malloc à %p\n", buffer);
  }
  fflush(stdout);
  system(s);
  /* les petits mallocs vont maintenant dans le tas des gros mallocs
   * jusqu'a ce qu'il deborde aussi, et on aura un 3ème tas.
   */

  return 0;
}
