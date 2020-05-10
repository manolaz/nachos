#ifdef CHANGED

#include "syscall.h"

void print(char *c)
{
	PutChar('\n');
	PutString(c);
#if 1
	PutChar('\n');
	PutChar('\n');
#endif
	ThreadExit();
}

void print2(char *c)
{
	PutChar('\n');
	PutString(c);
	PutChar('\n');
	ThreadExit();
}
/*
void reatThread(char c)
{
    //if(ThreadCreate(
}
*/
int main()
{
	ThreadCreate(print, "test thread 0");
	PutChar('t');
	PutChar('-');
	PutChar('\n');
	/*int i = 0;
	for(i = 0; i < 30; i++);
	    //creatThread('d');
	ThreadExit();*/
}

#endif // CHANGED
