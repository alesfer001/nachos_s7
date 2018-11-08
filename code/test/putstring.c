#include "syscall.h"

void print(char *s){
  int i;
  #if 1
  PutString(s);
  PutString("\n");
  #endif
}

int main(){
  print("aab"); //3
  print("aaaaaab"); //7
  print("aaaaaaab"); //8
  print("aaaaaaabaaaaaab"); //14
  print("aaaaaaabaaaaaaab"); //15
  Halt();
}
