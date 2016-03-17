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

char* fgets_or_exit(char *buffer , int size , FILE *stream){
  char *stock;	
  if((stock = fgets(buffer, size, stream)) == NULL) {
    exit(1);
  }
  return stock;
}



int check_and_open(const char* url, const char* document_root){
  
  char * chemin = malloc(strlen(url)+strlen(document_root));
  strcpy(chemin, document_root);
  strcat(chemin, url);

  struct stat file;

  stat(chemin ,&file);
		
  if (!S_ISREG(file.st_mode)) {
    return -1;
  } // fichier regulier
  
  int f;
  
  if ((f=open(chemin,O_RDONLY)) != 0){
    return -1;
  }

  return f;
}

int get_file_size(int fd){
  struct stat file;
  fstat(fd,&file);
  return file.st_size;
}

int copy(int in, int out){
  char buffer[10];
  while(read(in, buffer, 10)>0){
    if ((write(out, buffer, 10)) == -1){
      perror("Erreur");
    }
  }
  return 1;
}

struct mime tm[]={
  {"png","image/png"},
  {"html","text/html"},
  {NULL,NULL}
};

const char* getmime(const char* ext){
  int i;
  for(i = 0 ; tm[i].ext != NULL ; ++i){
    if (strcmp(tm[i].ext, ext) == 0){
      return tm[i].mime;
    }
  }
  return NULL;
}
