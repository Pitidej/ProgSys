#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>


int main ()
{
  int a = creer_serveur(8080);
  char message [1024] = "";
  int socket_client;
  int pid;
  while ((socket_client = accepte_client(a)) != -1){
    pid=fork();
    if(pid==0){
      FILE * fd = fdopen(socket_client, "w+");
      int i = read(socket_client, message, 1023);
      /*if (i != -1)
	{
	  //write(socket_client, message, i);
	  fprintf(fd, "%s%s", "Pawnee ", message);
	}
      else
	{
	  //write(socket_client, message, strlen(message));
	  fprintf(fd, "%s%s", "Pawnee ", message);
       }
      close(socket_client);
      exit(1);
      }*/
      fprintf(fd, "[Pawnee] %s", message);
      while (i){
	while (fgets(message, sizeof(message), fd) != NULL) {
	  printf("[Pawnee] %s", message);
	  fprintf(fd, "[Pawnee] %s", message);
	}
      }
    //close(socket_client);
  }
  }
  return 0;
}
