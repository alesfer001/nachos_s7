#include "syscall.h"

void print(char c){
  PutChar(c);
  ExitThread();
}

void print2(char c){
  PutChar(c);
}

int main(){
  CreateThread(print, 'A');
  CreateThread(print2, 'B');
  while(1);
  return 0;
}
