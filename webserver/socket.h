#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>

enum http_method {
HTTP_GET ,
HTTP_UNSUPPORTED ,
};
typedef struct
{
  
enum http_method method ;
int major_version ;
int minor_version ;
char* url ;
} http_request ;

#ifndef __SOCKET_H__
#define __SOCKET_H__
/** Cr�e une socket serveur qui �coute sur toute les interfaces IPv4
    de la machine sur le port pass� en param�tre. La socket retourn�e
    doit pouvoir �tre utilis�e directement par un appel � accept.

    La fonction retourne -1 en cas d'erreur ou le descripteur de la 
    socket cr��e. */

int creer_serveur(int port);
int accepte_client(int sock);
void initialiser_signaux(void);
void traitement_signal(int sig);
char* fgets_or_exit(char* buffer, int size, FILE* stream);
int parse_http_request(const char* request_line, http_request* request);
void skip_headers(FILE* client);
void send_status(FILE* client, int code, const char* reason_phrase);
void send_response(FILE *client, int code, const char* reason_phrase, const char* message_body);

#endif

