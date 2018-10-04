#include "syscall.h"

void print(char *s){
  int i;
  #if 1
  PutString(s);
  PutChar('\n');
  #endif
}

int main(){
  print("aaaabbbbbbbbbbbbbbbbc");
  Halt();
}
