#include "syscall.h"

void print(){
  int i;
  #if 1
  char s[3];
  GetString(s, 3);
  PutString(s);
  #endif
}

int main(){
  print();
}
