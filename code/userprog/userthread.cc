#include "userthread.h"
#include "thread.h"
#include "syscall.h"

int do_ThreadCreate(int f, int arg, int exit_value){
  int nth = currentThread->space->bitavail->Find();
  if(nth == -1){
    printf("Error: Unable to create thread, unsufficient memory!\n");
    return -1;
  }
  lock_nbThreads->P();
    Thread *my_thread = new Thread("New Thread");
    my_thread->mybit = nth;
    struct threadArgs *args = (struct threadArgs *) malloc(sizeof(struct threadArgs));
    args->func=f;
    args->arg=arg;
    args->exit_value=exit_value;
    // size enough bitmap

    currentThread->space->nbThreads++;
  lock_nbThreads->V();
  my_thread->Start(StartUserThread, args);
  return 1;
}

int do_ThreadExit(){
  lock_nbThreads->P();
    currentThread->space->bitavail->Clear(currentThread->mybit);
    currentThread->space->nbThreads--;
    if (currentThread->space->nbThreads == 0){
      interrupt->Halt();
    }
  lock_nbThreads->V();
  currentThread->Finish();
}


static void StartUserThread(void* schmurtz){
  struct threadArgs *myargs = (struct threadArgs *) schmurtz;
  DEBUG ('x', "Thread args f : %d, args: %d\n", myargs->func, myargs->arg);

  unsigned int i, size;
  size = currentThread->space->AllocateUserStack();

  for (i = 0; i < NumTotalRegs; i++)
    machine->WriteRegister (i, 0);

  // Initial program counter -- must be location of "Start"
  machine->WriteRegister (PCReg, myargs->func);
  machine->WriteRegister (4, myargs->arg);
  machine->WriteRegister (RetAddrReg, myargs->exit_value);

  // Need to also tell MIPS where next instruction is, because
  // of branch delay possibility
  machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

  // Set the stack register to the end of the address space, where we
  // allocated the stack; but subtract off a bit, to make sure we don't
  // accidentally reference off the end!
  machine->WriteRegister (StackReg, size - 16);

  DEBUG ('a', "Initializing stack register to 0x%x\n", size - 16);

  machine->Run();
  //free(myargs);

}
