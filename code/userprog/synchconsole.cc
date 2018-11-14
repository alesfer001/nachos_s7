#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore* readAvail;
static Semaphore* writeDone;
static Semaphore* putSem;
static Semaphore* getSem;
static void ReadAvailHandler(void* arg) {
  (void) arg; readAvail->V();
}
static void WriteDoneHandler(void* arg) {
  (void) arg; writeDone->V();
}

SynchConsole::SynchConsole(const char* in, const char* out){
  readAvail = new Semaphore("read avail", 0);
  writeDone = new Semaphore("write done", 0);
  putSem = new Semaphore("put", 1);
  getSem = new Semaphore("get", 1);
  console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, 0);
}
SynchConsole::~SynchConsole(){
  delete console;
  delete writeDone;
  delete readAvail;
}

void SynchConsole::SynchPutChar(int ch){
  putSem->P();
    console->PutChar(ch);
    writeDone-> P();
  putSem->V();
}
int SynchConsole::SynchGetChar(){
  getSem->P();
    readAvail-> P();
    int ch = console->GetChar();
  getSem->V();

  return ch;
}

void SynchConsole::SynchPutString(const char s[]){
  for(int i=0; s[i]!='\0'; i++){
    SynchPutChar(s[i]);
  }
}

void SynchConsole::SynchGetString(char* s, int n){
  int i;
  for(i=0; i < n; i++){
    s[i] = SynchGetChar();
    if(s[i] == '\n'){
      break;
    }
  }
  s[i+1] = '\0';
}

#endif // CHANGED
