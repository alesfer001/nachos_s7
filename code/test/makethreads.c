#include "syscall.h"

void print(char c){
  int i;
  for(i=0; i<5; i++)
    PutChar(c);
  ExitThread();
}

int main(){
  CreateThread(print, 'A');
  CreateThread(print, 'B');
  CreateThread(print, 'C');
  CreateThread(print, 'D');
  CreateThread(print, 'E');
  CreateThread(print, 'F');
  CreateThread(print, 'G');

  while(1);
  return 0;
}
