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
  int pid;
  initialiser_signaux();
  while ((socket_client = accepte_client(a)) != -1){
    pid=fork();
    if(pid==0){
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
  }
  initialiser_signaux();
  close(socket_client);
  return 0;
}
