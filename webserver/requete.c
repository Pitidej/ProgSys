#include "socket.h"
#include "signal.h"
#include "requete.h"
#include "fichier.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int parse_http_request(const char* request_line, http_request* request ){
  char* methode;
  char* url;
  char* version;
  char* struc = strdup(request_line);
  if((methode = strtok(struc," ")) == NULL){
    return 0;
  }
  if((url = strtok(NULL, " ")) == NULL) {
    return 0;
  }
  if((version = strtok(NULL, " ")) == NULL) {
    return 0;
  }
  if(strcmp(methode,"GET") == 0) {
    request->method = HTTP_GET;
  }else{
    request->method = HTTP_UNSUPPORTED;
  }
  if ((strcmp(version,"HTTP/1.0")==1) && (strcmp(version,"HTTP/1.1")==1)) {
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

char* rewrite_url(char *url){
  char *c = url;
  char *res = (char*)malloc(sizeof(char)*strlen(url));
  char *w = res;

  while(*c != '?' && *c != '\0'){
    *w = *c;
    c++;
    w++;
  }
  
  *w ='\0';
  return res;
  
}/* réalisée avec le groupe de kevin messian*/
