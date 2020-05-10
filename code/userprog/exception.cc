// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED

#include "userthread.h"
#include <iostream>

using namespace std;

int copyStringFromMachine(int from, char *to, unsigned size);
int copyStringToMachine(int to, char *from, unsigned size);
static void StartProcess (void * space);

#endif // CHANGED

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);

    switch (which)
      {
	case SyscallException:
          {
	    switch (type)
	      {
		case SC_Halt:
		  {
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
		    interrupt->Halt ();
		    break;
		  }

#ifdef CHANGED

		case SC_Exit:
		{
			DEBUG ('s', "exit\n");
			int exitCode = machine->ReadRegister (4);
			if (!exitCode)
				cout << "EXIT_SUCCESS (" << exitCode << ")" << endl;
			else
				cout << "EXIT_FAILURE (" << exitCode << ")" << endl;
		    interrupt->Halt ();
		    break;
		}
		case SC_PutChar:
		{
			DEBUG ('s', "putChar\n");
			int c = machine->ReadRegister (4);
		    synchconsole->SynchPutChar (c);
		    break;
		}
		case SC_PutString:
		{
			// on coupe la chaine MIPS en plusieurs bout de taille max
			DEBUG ('s', "putString\n");
			int from = machine->ReadRegister (4); 						// on pointe au debut de la chaine MIPS
			int nb_char = MAX_STRING_SIZE; 								// nombre de caractere dans le buffer
																		// que l'on initialise a taille max du buffer pour 
																		// rentrer dans le while
			char *buffer = (char *) malloc (sizeof (*buffer) * MAX_STRING_SIZE); // creation buffer de taille max
			int i = 0;													// permet de decaler d'un buffer dans la chaine MIPS
			while (nb_char == MAX_STRING_SIZE)
			{
				nb_char = copyStringFromMachine (from + i, buffer, MAX_STRING_SIZE); // on decale la tete de lecture
				i += nb_char;
				synchconsole->SynchPutString (buffer);
			}
			free (buffer); // on desalloue le buffer
			break;
		}
		case SC_GetChar:
		{
			DEBUG ('s', "getChar\n");
			int c = synchconsole->SynchGetChar();
			if (c != '\0' || c != '\n' || c != EOF)
				machine->WriteRegister (2, c);
			else
				interrupt->Halt ();
		    break;
		}
		case SC_GetString:
		{
			DEBUG ('s', "getString\n");
			int to = machine->ReadRegister (4);
			int size  = machine->ReadRegister (5);						// permet de recuperer la taille max de la chaine de
																		// caractere que l’on peut lire (egal a la taille du
																		// tableau de char dans test/getstring.c)
			int i = 0;
			int nb_char = MAX_STRING_SIZE;
			char *buffer = (char *) malloc (sizeof (*buffer) * MAX_STRING_SIZE);
			while (nb_char == MAX_STRING_SIZE && i < size)
			{
				synchconsole->SynchGetString (buffer, MAX_STRING_SIZE);	// on rempli le buffer avec la chaine de caractere 
																		// saisie par l'utilisateur
				nb_char = copyStringToMachine (to + i, buffer, MAX_STRING_SIZE);	// on decale la tete d'ecriture
				i += nb_char;
			}
			free (buffer); // on desalloue le buffer
			break;
		}
		case SC_PutInt:
		{
			DEBUG ('s', "putInt\n");
			int n = machine->ReadRegister (4);
			synchconsole->SynchPutInt(n);
			break;
		}
		case SC_GetInt:
		{
			DEBUG ('s', "getInt\n");
			int n;
			int addr = machine->ReadRegister (4);
			synchconsole->SynchGetInt (&n);
			machine->WriteMem (addr, 4, n); // on ecrit 4 octets (taille d'un int)
			break;
		}
		case SC_ThreadCreate:
		{
			DEBUG ('s', "threadCreate\n");
			int f = machine->ReadRegister (4);
			int arg = machine->ReadRegister (5);
			do_ThreadCreate (f, arg);
			break;
		}
		case SC_ThreadExit:
		{
			DEBUG ('s', "threadExit\n");
			do_ThreadExit ();
			break;
		}
		case SC_ForkExec:
		{
			DEBUG ('s', "forkexec\n");

			int taille_max_filename = 256;
			int from = machine->ReadRegister (4);
			char *filename = (char *) malloc (sizeof (*filename) * taille_max_filename);

			copyStringFromMachine (from, filename, taille_max_filename);
			//DEBUG ('s', "filename %s\n", filename);

			Thread *t = new Thread("process");
			OpenFile *executable = fileSystem->Open (filename);
		    AddrSpace *space;

		    if (executable == NULL)
			{
      			printf ("Unable to open file %s\n", filename);
				return;
			}

		    space = new AddrSpace (executable);
		    t->space = space;
			free (filename);
		    delete executable;      // close file
		    t->Start(StartProcess, t->space);
		    
			break;
		}

#endif // CHANGED

		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE);
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }

	case PageFaultException:
	  if (!type) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", type, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }

	default:
	  printf ("Unexpected user mode exception %d %d at PC %x\n", which, type, machine->registers[PCReg]);
	  ASSERT (FALSE);
      }
}

#ifdef CHANGED

// met dans un tableau de char (to), de taille size, une serie de caractere
// retourne le nombre de caractere placer dans le tableau
int copyStringFromMachine (int from, char *to, unsigned size)
{
	int value;
	unsigned int i = 0;
	while (to[i] != '\0' && i < size)
	{
		machine->ReadMem (from + i, 1, &value);
		to[i] = (char) value;
		if (to[i] == '\0') break; // si la chaine de caractere est < size
		i++;
	}
	return i;
}

// ecrit les caracteres du buffer from a partir de l'espace memoir to
int copyStringToMachine (int to, char *from, unsigned size)
{
	int value;
	unsigned int i = 0;
	while (from[i] != '\0' && from[i] != '\n' && i < size)
	{
		value = (int) from[i];
		machine->WriteMem (to + i, 1, value);
		i++;
	}
	machine->WriteMem (to + i, 1, '\0');
	return i;
}

static void
StartProcess (void * space)
{
	AddrSpace * adress; 
	adress = (AddrSpace *) space;
	adress->InitRegisters ();    // set the initial register values
    adress->RestoreState (); // load page table register
    currentThread->space = adress;

    machine->Run ();        // jump to the user progam
    ASSERT (FALSE);     // machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

#endif // CHANGED
