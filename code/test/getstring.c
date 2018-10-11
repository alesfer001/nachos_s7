#include "syscall.h"

void print(){
  int i;
  #if 1
  char s[51];
  GetString(s, 51);
  PutString(s);
  #endif
}

int main(){
  print();
}
