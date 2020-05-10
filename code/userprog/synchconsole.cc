#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Lock *mutexRead;
static Lock *mutexWrite;

static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

SynchConsole::SynchConsole(const char *in, const char *out)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	mutexRead = new Lock("mutexRead");
	mutexWrite = new Lock("mutexWrite");
	console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, 0);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
	delete mutexRead;
	delete mutexWrite;
}

void SynchConsole::SynchPutChar(int ch)
{
	mutexWrite->Acquire();

	console->PutChar(ch);
	writeDone->P();

	mutexWrite->Release();
}

int SynchConsole::SynchGetChar()
{
	mutexRead->Acquire();

	readAvail->P();
	int consoleGetChar = console->GetChar();

	mutexRead->Release();

	return consoleGetChar;
}

void SynchConsole::SynchPutString(const char *s)
{
	int i = 0;
	while(s[i] != '\0')
	{
		SynchPutChar(s[i]);
		i++;
	}
}

void SynchConsole::SynchGetString(char *s, int n)
{
	int i = 0;
	char c;

	while(i < n && c != EOF && c != '\n')
	{
		c = SynchGetChar();
		s[i] = c;
		i++;
	}
	s[i] = '\0';
}


void SynchConsole::SynchPutInt(int n)
{
	char *buffer = (char *)malloc(sizeof(*buffer) * MAX_STRING_SIZE);
	snprintf(buffer, MAX_STRING_SIZE, "%d", n);
	SynchPutString(buffer);
}

void SynchConsole::SynchGetInt(int *n)
{
	char *buffer = (char *)malloc(sizeof(*buffer) * MAX_STRING_SIZE);
	SynchGetString(buffer, MAX_STRING_SIZE);
	sscanf(buffer, "%d", n);
}

#endif // CHANGED
