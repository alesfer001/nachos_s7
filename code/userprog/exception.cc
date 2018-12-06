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

#include <stdlib.h>
#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "math.h"
#include "limits.h"
#include "userthread.h"
#include "userprocess.h"

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

  switch (which){
    case SyscallException:
    {
      switch (type)
      {
        case SC_Halt:
        {
          DEBUG ('s', "Shutdown, initiated by user program.\n");
          printf("HALT Syscall\n");
          interrupt->Halt ();
          break;
        }
        case SC_Exit:
        {
          printf("EXIT Syscall\n");
          interrupt->Halt ();
          break;
        }
        #ifdef CHANGED
        case SC_PutChar:
        {
          DEBUG ('s', "PutChar, initiated by user program.\n");
          int register_value = machine->ReadRegister (4);
          if(register_value == '\n')
             interrupt->Halt ();
	        mysynch_console->SynchPutChar(register_value);
          break;
        }
        case SC_PutString:
        {
          DEBUG ('s', "PutString, initiated by user program.\n");
          int register_value = machine->ReadRegister (4);
          char *buffer = (char *)malloc((MAX_STRING_SIZE) * sizeof(char));
          int nb_cop = copyStringFromMachine(register_value, buffer, MAX_STRING_SIZE);
          mysynch_console->SynchPutString(buffer);

          // If String is longer than MAX_STRING_SIZE
          int c;
          int cursor = 0;
          while(nb_cop == MAX_STRING_SIZE){
            cursor+=MAX_STRING_SIZE; // current reading position
            nb_cop = 0; // number of copied characters
            machine->ReadMem(register_value+cursor, sizeof(char), &c);
            if(c !='\0'){
              // Copy remaining string in buffer
              nb_cop = copyStringFromMachine(register_value+cursor, buffer, MAX_STRING_SIZE);
              mysynch_console->SynchPutString(buffer);
            }
          }
          free(buffer);
          break;
        }
        case SC_GetChar:
        {
          DEBUG ('s', "GetChar, initiated by user program.\n");
          int register_value = mysynch_console->SynchGetChar();
          if(register_value == EOF){
            interrupt->Halt ();
          }
          machine->WriteRegister(2, register_value);
          break;
        }
        case SC_GetString:
        {
          DEBUG ('s', "GetString, initiated by user program.\n");
          char *buffer = (char *)malloc((MAX_STRING_SIZE) * sizeof(char));
          int sout_addr = machine->ReadRegister (4);
          int size = machine->ReadRegister (5);

          int string_size = (MAX_STRING_SIZE > size) ? size: MAX_STRING_SIZE;

          mysynch_console->SynchGetString(buffer, string_size);

          int nb_cop = copyStringToMachine(sout_addr, buffer, string_size);
          int cursor=0;
          while(nb_cop == MAX_STRING_SIZE){
            size-=MAX_STRING_SIZE;
            string_size = (MAX_STRING_SIZE > size) ? size: MAX_STRING_SIZE;
            cursor+=MAX_STRING_SIZE; // current reading position
            nb_cop = 0; // number of copied characters
            
            mysynch_console->SynchGetString(buffer, string_size);
            nb_cop = copyStringToMachine(sout_addr+cursor, buffer, string_size);
          }
          free(buffer);
          break;
        }
        case SC_PutInt:
        {
          DEBUG ('s', "PutInt, initiated by user program.\n");
          char *buffer = (char *)malloc((MAX_INT_SIZE+2) * sizeof(char));
          int input = machine->ReadRegister (4);
	  // MAX_INT_SIZE + "-" + "\0"
          snprintf(buffer, (MAX_INT_SIZE+2) * sizeof(char) , "%d", input);
          mysynch_console->SynchPutString(buffer);

          free(buffer);
          break;
        }
        case SC_GetInt:
        {
          DEBUG ('s', "GetInt, initiated by user program.\n");

          char *buffer = (char *)malloc((MAX_INT_SIZE+1) * sizeof(char));
          // MAX_INT_SIZE + "-"
          mysynch_console->SynchGetString(buffer, MAX_INT_SIZE+1);

          int i;
          int sout_addr = machine->ReadRegister (4);
          sscanf(buffer, "%d", &i);
          machine->WriteMem(sout_addr, sizeof(i), i);
          break;
        }
        case SC_CreateThread:
        {
          DEBUG ('s', "CreateThread, initiated by user program.\n");
          int func = machine->ReadRegister (4);
          int args = machine->ReadRegister (5);
          int ext_value = machine->ReadRegister (6);
          do_ThreadCreate(func, args, ext_value);
          break;
        }
        case SC_ExitThread:
        {
          DEBUG ('s', "ExitThread, initiated by user program.\n");
          do_ThreadExit();
          break;
        }
        case SC_InitSem:
        {
          DEBUG ('s', "InitSem, initiated by user program.\n");
          int nth = semavail->Find();
          if(nth == -1){
            printf("Error: Unable to create Semaphore, unsufficient memory!\n");
            break;
          }
          int value = machine->ReadRegister (4);
          int semvalue = machine->ReadRegister (5);
          sems[nth] = new Semaphore("sem", semvalue);
          machine->WriteMem(value, sizeof(int), nth);
          break;
        }
        case SC_DeleteSem:
        {
          DEBUG ('s', "DeleteSem, initiated by user program.\n");
          int value = machine->ReadRegister (4);
          semavail->Clear(value);
          break;
        }
        case SC_PLock:
        {
          DEBUG ('s', "PLock, initiated by user program.\n");
          int value = machine->ReadRegister (4);
          sems[value]->P();
          break;
        }
        case SC_VUnlock:
        {
          DEBUG ('s', "VUnlock, initiated by user program.\n");
          int value = machine->ReadRegister (4);
          sems[value]->V();
          break;
        }
        case SC_ForkExec:
        {
          DEBUG ('s', "ForkExec, initiated by user program.\n");
          int register_value = machine->ReadRegister (4);
          char *filename = (char *)malloc((MAX_STRING_SIZE*10) * sizeof(char));
          int nb_cop = copyStringFromMachine(register_value, filename, MAX_STRING_SIZE*10);

          int exit_value = machine->ReadRegister (5);

          do_ProcessCreate(filename, exit_value);

          break;
        }
        #endif
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
