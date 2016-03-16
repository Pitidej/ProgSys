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

  const char * message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";

  http_request requete;
  int pars;
  
  while ((socket_client = accepte_client(a)) != -1){
    pid=fork();
    if(pid==0){
      FILE * fd = fdopen(socket_client, "w+");
      fgets_or_exit(message, sizeof(message), fd);
      pars = parse_http_request(message, &requete);
      skip_headers(fd);

      char* url1 = rewrite_url(requete.url);
      
      if (!pars){
	send_response(fd, 400,"Bad Request", "Bad request\r\n");
      } else if(requete.method == HTTP_UNSUPPORTED){
	send_response(fd ,405 ,"Method Not Allowed", "Method Not Allowed\r\n");
      } else if (strcmp(url1, "/") == 0){
	send_response(fd, 200, "OK" , message_bienvenue);
      } else {
	send_response(fd, 404, "Not Found", "Not Found\r\n");
      }  
      fclose(fd);
      free(url1);
    } else {
      close(socket_client);
    }
  }
  return 0;
}
