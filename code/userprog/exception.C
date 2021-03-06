#include <stdio.h>

class MonException {
};

void g()
{
  printf("D�but de g\n");

  throw MonException(); // g se plante !

  printf("Fin de g\n");
}

void f()
{
  printf("D�but de f\n");

  g(); // Si g se plante, f se plante aussi 
       // car on ne rattrape pas d'exception ici

  printf("Fin de f\n");
}

int main()
{
  printf("D�but de main\n");

  try {
    f();
  }
  catch(MonException &e) {
    printf("Exception rattrap�e dans le main : f s'est plant�e !\n");
  }
  catch(...) {
    printf("Exception inconnue rattrap�e dans le main...\n");
  }

  printf("Fin de main\n");

  return 0;
}
