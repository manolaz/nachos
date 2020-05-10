#ifdef CHANGED

#include "syscall.h"

void scan()
{
#if 1
	int c = GetChar();
	PutChar(c);
	PutChar('\n');
#endif
}

int main()
{
	scan();
	return 0;
}

#endif // CHANGED
