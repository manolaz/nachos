#ifdef CHANGED

#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "copyright.h"
#include "utility.h"
#include "console.h"

class SynchConsole:dontcopythis {
	public:
		SynchConsole(const char *readFile, const char *writeFile);
		~SynchConsole();

		void SynchPutChar(int ch);
		int SynchGetChar();

		void SynchPutString(const char *s);
		void SynchGetString(char *s, int n);
		
		void SynchPutInt(int n);
		void SynchGetInt(int *n);

	private:
		Console *console;
};

#endif

#endif // CHANGED
