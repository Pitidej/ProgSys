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
  const char *mess404 = "HTTP/1.1 404 Not found\r\nConnection: Close\r\nContent-Length: 17 \r\n\n400 Bad request\r";
  const char *mess200 = "HTTP/1.1 200 OK\nContent-Length : 23\r\n";

  const char * message_bienvenue = "Bonjour, bienvenue sur mon serveur. \n Il est pas interessant mais tant qu'il fonctionne, ca va. \n Sauf que M.Carle ne sera pas satistait. \n En l'occurence, on ne se pose pas assez de question. \n On est con. \n On ne sait pas rechercher dans la doc. \n J'espere que ca fait 10 lignes. \n" ;
  char test[1024];
  char *methode;
  char *url;
  char *ressources;
  char *spl;
  while ((socket_client = accepte_client(a)) != -1){
    pid=fork();
    if(pid==0){
      FILE * fd = fdopen(socket_client, "w+");
      fgets(message, sizeof(message), fd);
      while (fgets(test,sizeof(test),fd)!=NULL && test[0]!='\r' && test[0]!='\n'){
      }
      //int i = read(socket_client, message, 1023);
      methode = strtok(message, " ");
      url = strtok(NULL, " ");
      ressources = strtok(NULL, " ");
      spl = strtok(NULL, " ");
      
      //printf("%s\n%s\n%s",methode, ressources, spl);
      
      if ((strcmp(methode,"GET")==0) && ((strstr(ressources,"HTTP/1.0")==0) || (strstr(ressources,"HTTP/1.1")==0)) && (spl == NULL) ){
	if (strcmp(url,"/")==0){
	  fprintf(fd, "[pawneeeeee] %s \n", mess200);
	  fflush(fd);
	  fprintf(fd,"%s\n",message_bienvenue);
	  fclose(fd);
	} else {
	  fprintf(fd,"%s\n", mess404);
	  fclose(fd);
	}
      } else {
	fprintf(fd,"%s\n",mess400);
	fflush(fd);
	fclose(fd);
      }
       
    } else {
      close(socket_client);
    }
  }
return 0;
}
