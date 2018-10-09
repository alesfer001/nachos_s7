#include "syscall.h"

void print(char *s){
  int i;
  #if 1
  PutString(s);
  PutString("\n");
  #endif
}

int main(){
  print("aaaabbbbbbbbbbbbbbbc"); //20
  Halt();
}
