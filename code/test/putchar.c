#include "syscall.h"

void PutChar(char s);

void print(char c, int n){
  int i;
  #if 1
  for (i = 0; i < n; i++) {
    PutChar(c + i);
  }
  PutChar('\n');
  #endif
}

int main(){
  print('a',4);
  Halt();
}

void PutChar(char s){
  return;
}
