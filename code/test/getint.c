#include "syscall.h"

void print(){
  #if 1
  int n=0;
  GetInt(&n);
  PutInt(n);
  PutString("\n");
  #endif
}

int main(){
  print();
}
