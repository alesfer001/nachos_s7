#include <stdio.h>

class MonException {
};

void g()
{
  printf("Début de g\n");

  throw MonException(); // g se plante !

  printf("Fin de g\n");
}

void f()
{
  printf("Début de f\n");

  g(); // Si g se plante, f se plante aussi 
       // car on ne rattrape pas d'exception ici

  printf("Fin de f\n");
}

int main()
{
  printf("Début de main\n");

  try {
    f();
  }
  catch(MonException &e) {
    printf("Exception rattrapée dans le main : f s'est plantée !\n");
  }
  catch(...) {
    printf("Exception inconnue rattrapée dans le main...\n");
  }

  printf("Fin de main\n");

  return 0;
}
