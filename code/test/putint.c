#ifdef CHANGED

#include "syscall.h"

void print(int n)
{
#if 1
	PutInt(n);
	PutChar('\n');
#endif
}

int main()
{
	print(1054542154);
	return 0;
}

#endif // CHANGED
