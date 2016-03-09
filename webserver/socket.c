#include "socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>


int creer_serveur(int port){

  /* Création de la socket */

  int socket_serveur ;
  socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
  if ( socket_serveur == -1)
    {
      perror("socket_serveur");
      /* traitement de l ’ erreur */
    }

  initialiser_signaux();
  
  /* Attachement de la socket */

  struct sockaddr_in saddr ;
  saddr.sin_family = AF_INET ; /* Socket ipv4 */
  saddr.sin_port = htons(port); /* Port d’écoute */
  saddr.sin_addr.s_addr = INADDR_ANY ; /* écoute sur toutes les interfaces */

  int optval = 1;
  if (setsockopt(socket_serveur, SOL_SOCKET , SO_REUSEADDR , &optval , sizeof(int)) == -1){
    perror("Can not set SO_REUSEADDR option");
  }
  
  if (bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
    perror("bind socker_serveur");
    /* traitement de l ’ erreur */
  }

  /* Lecture de la connexion */

  if (listen(socket_serveur, 10) == -1) {
    perror ( " listen socket_serveur " );
    /* traitement d ’ erreur */
  }

  

  return socket_serveur;

}

int accepte_client(int sock){
  
  /* Accepter une connexion */

  int socket_client ;
  socket_client = accept(sock, NULL, NULL);
  
  //FILE * fd=fdopen(socket_client,"w+");
  
  if (socket_client == -1) {
    perror("accept");
    /* traitement d’ erreur */
  }
  sleep(1);
  
  return socket_client;
}

void traitement_signal(int sig)
{
  printf("Signal %d recu\n", sig);
  wait(&sig);
  if(WIFSIGNALED(sig)){
    printf("%d\n", WTERMSIG(sig));
  }
}

void initialiser_signaux (void)
{
  if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
    perror("signal");
  }
  
  struct sigaction sa;
  
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD,&sa,NULL) == -1)
    {
      perror ("sigaction(SIGCHLD)");
    }
}

char* fgets_or_exit(char *buffer , int size , FILE *stream){
  char *stock;	
  if((stock = fgets(buffer, size, stream)) == NULL) {
    exit(1);
  }
  return stock;
}

int parse_http_request(const char* request_line, http_request* request ){
  char* methode;
  char* url;
  char* http_version;
  char* struc = strdup(request_line);
  if((methode = strtok (struc," ")) == NULL){
    return 0;
  }
  if((url = strtok (NULL, " ")) == NULL) {
    return 0;
  }
  if((http_version = strtok (NULL, " ")) == NULL) {
    return 0;
  }
  if(strcmp(methode,"GET") == 0) {
    request->method = HTTP_GET;
  }else{
    request->method = HTTP_UNSUPPORTED;
  }
  if ((strcmp(http_version,"HTTP/1.0")!=0) && (strcmp(http_version,"HTTP/1.1")!=0)) {
    return 0;
  }
  request->url = url;
  return 1;
}

void skip_headers(FILE *client){
  char cumul[1024];
  while(cumul[0] != '\r' && cumul[1] != '\n') {
    fgets_or_exit(cumul, sizeof(cumul), client);
  }
}

void send_status(FILE *client, int code, const char *reason_phrase){
  char status[256];
  sprintf(status, "HTTP/1.1 %d %s\r\n", code, reason_phrase);
  fprintf(client, status);
}

void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body){
  send_status(client, code, reason_phrase);
  if (message_body != NULL){
    char taille[512];
    sprintf(taille, "Content-Length: %zu\r\n", strlen(message_body));
    fprintf(client, "Content-Type: text/html\r\n");
    fprintf(client, taille);
    fprintf(client, "\r\n");
    fprintf(client, message_body);
  }
  fprintf(client, "\r\n");
}
