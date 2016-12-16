#ifndef MIME_H
#define MIME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <fcntl.h>
#include <errno.h>



typedef struct {
  char* nom;
  char* extension;
} mr_mime;



mr_mime* creer_mr_mime(char* name ,char* ext);
int read_line (int fd, char *ret);
int creerTab(mr_mime*** tab,int* taille);
#endif