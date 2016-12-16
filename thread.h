#ifndef THREAD_H
#define THREAD_H
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <pthread.h>
#include <fcntl.h>

#include "parser.h"

#ifndef REPONSE_THREAD
typedef struct{
	int sock;
	struct sockaddr_in csin;
	char* buffer;
	int* tab;
	int num;
	pthread_mutex_t* mux;
}reponse_thread;
#endif
/*REPONSE_THREAD*/

#ifndef MESSAGE_HTML
#define MESSAGE_HTML "<html><header><title>JE SUIS UN TITRE</title></header><body>je suis un message</body></html>"
#endif
/*MESSAGE_HTML */


#ifndef TAILLE_BUFFER
#define TAILLE_BUFFER 512
#endif
/*TAILLE_BUFFER*/


#ifndef NAME_MMAP
#define NAME_MMAP "memThread"
#endif
/*NAME_MMAP*/

#ifndef MAP_MUT
#define MAP_MUT "mapu"
#endif
/*MAP_MUT*/

void* routine(void* arg);
void* setmmap_thread(int taille);
void* mapmutex();
#endif 
/*THREAD_H*/