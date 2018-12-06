#include "syscall.h"

void print(char *s){
  int i;
  #if 1
  PutString(s);
  #endif
}

int main(){
  print("aab\n"); //3
  print("aaaaaab\n"); //7
  print("aaaaaaab\n"); //8
  print("aaaaaaabaaaaaab\n"); //14
  print("aaaaaaabaaaaaaab\n"); //15
}
