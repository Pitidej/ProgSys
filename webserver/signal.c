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
