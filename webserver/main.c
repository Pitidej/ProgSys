#include <stdio.h>
#include <string.h>
#include "socket.h"
int main ()
{
  int a = creer_serveur(8080);

  while (1){
    accepte_client(a);
  }
  
  return 0;
}
