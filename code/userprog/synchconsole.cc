#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore* readAvail;
static Semaphore* writeDone;

static void ReadAvailHandler(void* arg) {
  (void) arg; readAvail->V();
}
static void WriteDoneHandler(void* arg) {
  (void) arg; writeDone->V();
}

SynchConsole::SynchConsole(const char* in, const char* out){
  readAvail = new Semaphore("read avail", 0);
  writeDone = new Semaphore("write done", 0);
  console = new Console(in, out, ReadAvailHandler, writeDoneHandler, 0);
}
SynchConsole::~SynchConsole(){
  delete console;
  delete writeDone;
  delete readAvail;
}

void SynchConsole::SynchPutChar(int ch){
// ...
}
int SynchConsole::SynchGetChar(){
  int ch = incoming;

  incoming = EOF;
  return ch;
}
void SynchConsole::SynchPutString(const char s[]){
// ...
}
void SynchConsole::SynchGetString(char* s, int n){
// ...
}

#endif // CHANGED
