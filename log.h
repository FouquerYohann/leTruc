#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define LOGFILENAME "./tmp/http3364803.log"


/* Mutex to protect logfile access */
static pthread_mutex_t mutex_logger = PTHREAD_MUTEX_INITIALIZER;




typedef struct{
	char* client_ip;
	char* request_date;
	char* thread_id;
	char* request_first_line;
	char* request_return;
	char* file_size;
}logInfo;


logInfo* creer_log(char* ip, char* thread_self, char* first, char* ret, char* size);
int     write_log(logInfo* log);


#endif