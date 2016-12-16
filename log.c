#include "log.h"


logInfo* creer_log(char* ip, char* thread_self, char* first, char* ret, char* size){
	logInfo* log=(logInfo*)malloc(sizeof(logInfo));
	log->client_ip          =strdup(ip);
	log->thread_id          =strdup(thread_self);
	log->request_first_line =strdup(first);
	log->request_return     =strdup(ret);
	log->file_size          =strdup(size);

	log->request_date=(char*)malloc(26*sizeof(char));
	time_t mytime;
    mytime = time(NULL);
    ctime_r(&mytime,log->request_date);
    log->request_date[strlen(log->request_date) -1]='\0';

    return log;
}


int write_log(logInfo* log){
	char* str_print=(char*)malloc(512*sizeof(char));
	str_print[0]='\0';
	strcat(str_print,log->client_ip);
	strcat(str_print,";");
	strcat(str_print,log->request_date);
	strcat(str_print,";");
	strcat(str_print,log->thread_id);
	strcat(str_print,";");
	strcat(str_print,log->request_first_line);
	strcat(str_print,";");
	strcat(str_print,log->request_return);
	strcat(str_print,";");
	strcat(str_print,log->file_size);
	strcat(str_print,"\n");

	int size=strlen(str_print);


	int fd;

	pthread_mutex_lock(&mutex_logger);

	if( (fd=open(LOGFILENAME,O_CREAT |O_APPEND |O_WRONLY, 0666)) <0){
		perror("open()");
		return 1;
	}

	

	if(write(fd,str_print,size) != size){
		perror("write()");
		return 2;
	}

	pthread_mutex_unlock(&mutex_logger);

	return 0;

}
	
