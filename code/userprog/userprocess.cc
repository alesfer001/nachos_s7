#include "userprocess.h"

int do_ProcessCreate(char *filename, int exit_value){
  int nth = currentThread->space->bitavail->Find();
  if(nth == -1){
    printf("Error: Unable to create thread, unsufficient memory!\n");
    return -1;
  }
  lock_nbThreads->P();
    Thread *my_thread = new Thread("New Thread");
    my_thread->mybit = nth;
    OpenFile *executable = fileSystem->Open(filename);
    AddrSpace *space;

    if (executable == NULL){
      printf ("Unable to open file %s\n", filename);
      return -1;
    }
    try{
    space = new AddrSpace (executable);
    }
    catch(AddrException &e){
      printf("Page space unavailable!\n");
      return -1;
    }
    currentThread->space->nbThreads++; // nb_process++ (global)

    struct procArgs *args = (struct procArgs *) malloc(sizeof(struct procArgs));
    args->space=space;
    args->exit_value=exit_value;
  lock_nbThreads->V();
  my_thread->Start(StartUserProcess, args);
  return 1;
}


void StartUserProcess(void *schmurtz){
  struct procArgs *args = (struct procArgs *) schmurtz;
  currentThread->space = args->space;

  currentThread->space->InitRegisters ();	// set the initial register values
  currentThread->space->RestoreState ();	// load page table register

  machine->WriteRegister (RetAddrReg, args->exit_value);

  machine->Run ();		// jump to the user progam
}
