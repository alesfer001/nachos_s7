#include "syscall.h"

void print(char c){
  PutChar(c);
  PutChar('c');
  ExitThread();
}

int main(){
  CreateThread(print, 'a');
  //PutChar('f');
  while(1){}
  return 0;
}
