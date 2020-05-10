#ifdef CHANGED

#include "syscall.h"

void scan()
{
	int i;
#if 1
	GetInt(&i);
	PutInt(i);
	PutChar('\n');
#endif
}

int main()
{
	scan();
	return 0;
}

#endif // CHANGED
