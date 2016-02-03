#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main ()
{
  int a = creer_serveur(8080);
  char message [1024] = "";
  int socket_client;
  while (1){
    socket_client = accepte_client(a);
    int i = read(socket_client, message, 1023);
    if (i != -1)
      {
	write(socket_client, message, i);
      }
    else
      {
	write(socket_client, message, strlen(message));
      }
  }
  
  return 0;
}
