#include "syscall.h"

void print(int n){
  #if 1
  PutInt(n);
  PutString("\n");
  #endif
}

int main(){
  print(2147483647);
  print(2147483648);
  print(-2147483648);
  print(-2147483649);
}
