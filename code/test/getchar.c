#include "syscall.h"

void print(){
  #if 1
  int g = GetChar();
  PutChar(g);
  #endif
}

int main(){
  print();
}
