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
  
  const char * message_bienvenue = "Bonjour, bienvenue sur mon serveur. \n Il est pas interessant mais tant qu'il fonctionne, ca va. \n Sauf que M.Carle ne sera pas satistait. \n En l'occurence, on ne se pose pas assez de question. \n On est con. \n On ne sait pas rechercher dans la doc. \n J'espere que ca fait 10 lignes. \n" ;
  /* Accepter une connexion */

  int socket_client ;
  socket_client = accept(sock, NULL, NULL);
  if (socket_client == -1) {
    perror("accept");
    /* traitement d’ erreur */
  }
  sleep(3);
  write(socket_client, message_bienvenue, strlen(message_bienvenue));

  return socket_client;
}

void traitement_signal(int sig)
{
  printf("Signal %d recu\n", sig);
  wait(&sig);
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
