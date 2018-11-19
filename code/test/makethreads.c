#include "syscall.h"

int wait_prod;
int wait_cons;
int mutex;
char tube;

void prod(char e){
  PLock(wait_prod);
    PLock(mutex);
      tube = e;
    VUnlock(mutex);
  VUnlock(wait_cons);
}

void cons(char e){
  PLock(wait_cons);
    PLock(mutex);
      PutChar(tube);
    VUnlock(mutex);
  VUnlock(wait_prod);
  //ExitThread();
}

int main(){
  InitSem(&wait_prod, 1);
  InitSem(&wait_cons, 0);
  InitSem(&mutex, 1);

  CreateThread(prod, 'A');
  CreateThread(cons, ' ');
  CreateThread(prod, 'B');
  CreateThread(cons, ' ');
  DeleteSem(wait_prod);
  DeleteSem(wait_cons);
  while(1);
  return 0;
}
