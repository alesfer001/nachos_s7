#include "syscall.h"

void print(char *s){
  int i;
  #if 1
  PutString(s);
  #endif
}

int main(){
  print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"); //50
  Halt();
}
