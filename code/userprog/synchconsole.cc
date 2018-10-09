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
  console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, 0);
}
SynchConsole::~SynchConsole(){
  delete console;
  delete writeDone;
  delete readAvail;
}

void SynchConsole::SynchPutChar(int ch){
    console->PutChar(ch);
    writeDone-> P();
}
int SynchConsole::SynchGetChar(){
  readAvail-> P();
  int ch = console->GetChar();
  return ch;
}

void SynchConsole::SynchPutString(const char s[]){
  for(int i=0; s[i]!='\0'; i++){
    SynchPutChar(s[i]);
  }
}

void SynchConsole::SynchGetString(char* s, int n){
  for(int i=0; i < n; i++){
    s[i] = SynchGetChar();
    printf("%c", s[i]);
  }
  printf("\n");
}

#endif // CHANGED
