#include "syscall.h"

void print(){
  int i;
  #if 1
  char s[51];
  GetString(s, 14);
  PutString(s);
  PutString("\n");
  #endif
}

int main(){
  print();
}
