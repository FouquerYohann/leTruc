#ifndef SERVEUR_H
#define SERVEUR_H
#include <sys/mman.h>

#include "thread.h"
#include "mime.h"


#ifndef TAILLE_BUFFER
#define TAILLE_BUFFER 512
#endif


extern mr_mime** mime_tab;
extern int taille_mime_tab;

static pthread_mutex_t mu_tex = PTHREAD_MUTEX_INITIALIZER;

int init_sock(int nb_client,int port);
#endif 
/* SERVEUR_H */