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
  const char *mess400 = "HTTP/1.1 400 Bad Request\r\nConnection: Close\r\nContent-Length: 17 \r\n\n400 Bad request\r";
  const char *mess200 = "HTTP/1.1 200 OK\r\nContent-Length: ";
  //char tok [1024];
  char *methode;
  char *ressources;
  char *spl;
  
  while ((socket_client = accepte_client(a)) != -1){
    pid=fork();
    if(pid==0){
      FILE * fd = fdopen(socket_client, "w+");
      fgets(message, sizeof(message), fd);
      //int i = read(socket_client, message, 1023);

      methode = strtok(message, " ");
      strtok(NULL, " ");
      ressources = strtok(NULL, " ");
      
      spl = strtok(NULL, " ");
      
      //printf("%s\n%s\n%s",methode, ressources, spl);
      
      if ((strcmp(methode,"GET")==0) && (ressources != NULL) && (spl == NULL)){
	if ((strcmp(ressources,"HTTP/1.0")==0) || (strcmp(ressources,"HTTP/1.1")==0)){
	  fprintf(fd, "%s\n", mess200 );
	  fflush(fd);
	  fclose(fd);
	}
      } else {
	fprintf(fd,"%s\n",mess400);
	fflush(fd);
	fclose(fd);
      }

      /*printf("[Pawnee] %s", message);
      fprintf(fd, "[Pawnee] %s", message);
      while (i){
	while (fgets(message, sizeof(message), fd) != NULL) {
	  printf("[Pawnee] %s", message);
	  fprintf(fd, "[Pawnee] %s", message);
	}
      }
      
      if (i != -1)
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
      
      //fprintf(fd, "[Pawnee] %s", message);
      
    //close(socket_client);
    }
  }
  return 0;
}
