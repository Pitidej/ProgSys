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

char* fgets_or_exit(char *buffer , int size , FILE *stream);
int check_and_open(const char* url, const char* document_root);
int get_file_size(int fd);
int copy(int in, int out);

struct mime{
  const char* ext;
  const char* mime;
};

const char* getmime(const char* ext);
