#ifdef CHANGED

#include "syscall.h"

void scan()
{
	const unsigned int TAILLE_MAX = 200;
	char s[TAILLE_MAX];
#if 1
	GetString(s, TAILLE_MAX);
	PutString(s);
	PutChar('\n');
#endif
}

int main()
{
	scan();
	return 0;
}

#endif // CHANGED
