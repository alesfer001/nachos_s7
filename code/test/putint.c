#include "syscall.h"

void print(int n){
  #if 1
  PutInt(n);
  PutString("\n");
  #endif
}

int main(){
  int n = -2147483648;
  print(n);
}
